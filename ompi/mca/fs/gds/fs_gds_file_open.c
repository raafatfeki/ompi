/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2017 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2008-2018 University of Houston. All rights reserved.
 * Copyright (c) 2015-2018 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2016-2017 IBM Corporation. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */


#include "ompi_config.h"
#include "fs_gds.h"

#include <fcntl.h>
#include <sys/stat.h>
#include "mpi.h"
#include "ompi/constants.h"
#include "ompi/mca/fs/base/base.h"
#include "ompi/mca/fs/fs.h"
#include "ompi/communicator/communicator.h"
#include "ompi/info/info.h"
#include "opal/util/path.h"

/*
 *	file_open_gds
 *
 *	Function:	- opens a new file
 *	Accepts:	- same arguments as MPI_File_open()
 *	Returns:	- Success if new file handle
 */

// Raafat: Check if we need to close opened files in errors here: Probably not

int
mca_fs_gds_file_open (struct ompi_communicator_t *comm,
        		      const char* filename,
        		      int access_mode,
        		      struct opal_info_t *info,
        		      ompio_file_t *fh)
{
    int amode_direct, amode, perm;
    int ret = OMPI_SUCCESS;

    CUfileError_t status;
    CUfileDescr_t cf_descr;
    // Raafat: Don't forget to choose whether to print error only or exit also 
    // Raafat: in example used 0664: just check if there is a particular reason for that
    perm  = mca_fs_base_get_file_perm(fh);
    amode = mca_fs_base_get_file_amode(fh->f_rank, access_mode);


    amode_direct = amode | O_DIRECT;
    errno = 0;
    fh->fd = -1;
    fh->fd_direct = -1;

    if (OMPIO_ROOT == fh->f_rank) {
	    fh->fd_direct = open (filename, amode_direct, perm);
        if ( 0 > fh->fd_direct ) {
            ret = mca_fs_base_get_mpi_err(errno);
        }
    }

    comm->c_coll->coll_bcast ( &ret, 1, MPI_INT, 0, comm, comm->c_coll->coll_bcast_module);
    if ( OMPI_SUCCESS != ret ) {
        return ret;
    }

    if (OMPIO_ROOT != fh->f_rank) {
        fh->fd_direct = open (filename, amode_direct, perm);
        if ( 0 > fh->fd_direct) {
            return mca_fs_base_get_mpi_err(errno);
        }
    }

    // Same file must not be opened twice with MPI_MODE_EXCL
    // Check if is it safe to keep MPI_MODE_CREATE
    amode &= (~MPI_MODE_EXCL);
    fh->fd = open (filename, amode, perm);
    if ( 0 > fh->fd) {
        return mca_fs_base_get_mpi_err(errno);
    }

    memset((void *)&cf_descr, 0, sizeof(CUfileDescr_t));
    cf_descr.handle.fd = fh->fd_direct;
    // Raafat: check this
    cf_descr.type = CU_FILE_HANDLE_TYPE_OPAQUE_FD;

    fh->f_fs_ptr = malloc(sizeof(CUfileHandle_t));
    if (NULL == fh->f_fs_ptr) {
        opal_output (1, "OUT OF MEMORY\n");
        return OMPI_ERR_OUT_OF_RESOURCE;
    }

    status = cuFileHandleRegister((CUfileHandle_t *)fh->f_fs_ptr, &cf_descr);
    if (CU_FILE_SUCCESS != status.err) {
        CUFILE_ERROR_OUTPUT(1, "GDS fs: cuFileHandleRegister error", status);
        free(fh->f_fs_ptr);
        fh->f_fs_ptr = NULL;
        return OMPI_ERROR;
    }

    return OMPI_SUCCESS;
}
