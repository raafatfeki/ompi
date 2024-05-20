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
 * Copyright (c) 2008-2011 University of Houston. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */


#include "ompi_config.h"
#include "fs_gds.h"

#include <fcntl.h>
#include <unistd.h>
#include "mpi.h"
#include "ompi/constants.h"
#include "ompi/mca/fs/fs.h"

/*
 *	file_close_gds
 *
 *	Function:	- closes a new file
 *	Accepts:	- file handle
 *	Returns:	- Success if file closed
 */

int
mca_fs_gds_file_close (ompio_file_t *fh)
{
    CUfileError_t status;

    fh->f_comm->c_coll->coll_barrier (fh->f_comm,
                                     fh->f_comm->c_coll->coll_barrier_module);
    close(fh->fd);

    if (NULL != fh->f_fs_ptr) {
        cuFileHandleDeregister(*((CUfileHandle_t *)fh->f_fs_ptr));
        free(fh->f_fs_ptr);
        fh->f_fs_ptr = NULL;
    }

    close(fh->fd_direct);

    return OMPI_SUCCESS;
}
