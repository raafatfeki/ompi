/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2011 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2008-2020 University of Houston. All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "fbtl_gds.h"
#include "ompi/mca/common/ompio/common_ompio_buffer.h"

ssize_t mca_fbtl_gds_preadv(ompio_file_t *fh)
{
    int i, is_gpu, is_managed;
    ssize_t bytes_read = 0, total_bytes_read = 0;
    size_t nbytes, rem, diff;
    char *device_buf = NULL;
    void *host_buf = NULL;
    CUfileError_t status;

    if (NULL == fh->f_io_array) {
        return OMPI_ERROR;
    }

    mca_common_ompio_check_gpu_buf(fh, fh->f_io_array[0].memory_address, 
                                   &is_gpu, &is_managed);
    if(!is_gpu) {
        opal_output_verbose(10, 1,
                            "fbtl:gds:pwritev: Not GPU Memory - Using fbtl: %s",
                            mca_fbtl_gds_cpumem_fbtl_name);
        return mca_fbtl_gds_cpumem_fbtl->fbtl_preadv(fh);
    }

    // TODO: We can allocate this at the component level.
    host_buf = malloc(mca_fbtl_gds_directio_block_size - 1);
    if(NULL == host_buf) {
        return OMPI_ERR_OUT_OF_RESOURCE;
    }

    /* 
    ** To use direct I/O :
    ** 1. make sure fh->f_io_array[i].offset starts at a block boundary
    ** 2. make sure fh->f_io_array[i].length is a multiple of the block size
    ** 3. make sure fh->f_io_array[i].memory_address is aligned to a page size
    */
    for (i=0; i<fh->f_num_of_io_entries; i++) {
        // Step 1: Make sure offset is aligned.
        if ((OMPI_MPI_OFFSET_TYPE )fh->f_io_array[i].offset % mca_fbtl_gds_directio_block_size) {
            diff = mca_fbtl_gds_directio_block_size - 
                ((OMPI_MPI_OFFSET_TYPE)fh->f_io_array[i].offset % mca_fbtl_gds_directio_block_size);

            if (fh->f_io_array[i].length < diff) {
                diff = fh->f_io_array[i].length;
            }

            // Read partial block to Host memory.
            total_bytes_read += pread(fh->fd, host_buf, 
                                     diff, 
                                     (off_t) fh->f_io_array[i].offset);
            if (0 > total_bytes_read){
                opal_output(1, "fbtl_gds_preadv: error in pread: %s\n", strerror(errno));
                return OMPI_ERROR;
            }

            // To Check: I think cufile handles now non-gpu memory- Do we need to do this?
            opal_accelerator.mem_copy(MCA_ACCELERATOR_NO_DEVICE_ID, MCA_ACCELERATOR_NO_DEVICE_ID, 
                                      fh->f_io_array[i].memory_address, host_buf, 
                                      diff, MCA_ACCELERATOR_TRANSFER_HTOD);

            fh->f_io_array[i].memory_address = ((char *) fh->f_io_array[i].memory_address) + diff;
            fh->f_io_array[i].offset = (IOVBASE_TYPE *)(intptr_t)((off_t)fh->f_io_array[i].offset + diff);
            fh->f_io_array[i].length -= diff;
        }

        // Step 2: Make sure length is a multiple of block size.
        rem = fh->f_io_array[i].length % mca_fbtl_gds_directio_block_size;
        nbytes = fh->f_io_array[i].length - rem;

        if (0 < nbytes) {
            device_buf = fh->f_io_array[i].memory_address;
            status = cuFileBufRegister(device_buf, nbytes, 0);
            if (CU_FILE_SUCCESS != status.err) {
                CUFILE_ERROR_OUTPUT(1, "fbtl_gds_preadv: error in cuFileBufRegister", status);
            }

            bytes_read = cuFileRead(*(CUfileHandle_t *)(fh->f_fs_ptr), 
                                    device_buf, 
                                    nbytes,
                                    (off_t )fh->f_io_array[i].offset, 0);
            
            if (bytes_read < 0) {
                opal_output(1, "fbtl_gds_preadv: error in cuFileRead: %s\n", strerror(errno) );
                return OMPI_ERROR;
            }

            total_bytes_read += bytes_read;

            status = cuFileBufDeregister(device_buf);
            if (CU_FILE_SUCCESS != status.err) {
                CUFILE_ERROR_OUTPUT(1, "fbtl_gds_preadv: error in cuFileBufDeregister", status);
            }
        }

        // Step 3: Read remaining partial block to Host memory.
        if (rem > 0) {         
            bytes_read = pread(fh->fd, host_buf, 
                               rem,
                               (off_t )fh->f_io_array[i].offset + nbytes);
            if (bytes_read < 0) {
                opal_output(1, "fbtl_gds_preadv: error in pread: %s\n", strerror(errno));
                return OMPI_ERROR;
            }

            opal_accelerator.mem_copy(MCA_ACCELERATOR_NO_DEVICE_ID, MCA_ACCELERATOR_NO_DEVICE_ID, 
                                      ((char *)fh->f_io_array[i].memory_address) + nbytes, 
                                      host_buf, rem, MCA_ACCELERATOR_TRANSFER_HTOD);

            total_bytes_read += bytes_read;
        }
    }

    free(host_buf);
    return total_bytes_read;
}