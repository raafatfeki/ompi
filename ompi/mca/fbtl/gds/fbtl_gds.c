/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2006 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2008-2020 University of Houston. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 * These symbols are in a file by themselves to provide nice linker
 * semantics. Since linkers generally pull in symbols by object fules,
 * keeping these symbols as the only symbols in this file prevents
 * utility programs such as "ompi_info" from having to import entire
 * modules just to query their version and parameters
 */

#include "ompi_config.h"
#include "mpi.h"

#include <unistd.h>
#include <sys/uio.h>

#include "ompi/mca/fbtl/fbtl.h"
#include "ompi/mca/fbtl/base/base.h"
#include "ompi/mca/fbtl/gds/fbtl_gds.h"

/*
 * *******************************************************************
 * ************************ actions structure ************************
 * *******************************************************************
 */
static mca_fbtl_base_module_1_0_0_t gds =  {
    mca_fbtl_gds_module_init,       /* initalise after being selected */
    mca_fbtl_gds_module_finalize,   /* close a module on a communicator */
    mca_fbtl_gds_preadv,            /* blocking read */
    NULL,                           /* non-blocking read */
    mca_fbtl_gds_pwritev,           /* blocking write */
    NULL,                           /* non-blocking write */
    NULL,                           /* module specific progress */
    NULL,                           /* free module specific data items on the request */
    mca_fbtl_base_check_atomicity   /* check whether atomicity is supported on this fs */
};
/*
 * *******************************************************************
 * ************************* structure ends **************************
 * *******************************************************************
 */

int mca_fbtl_gds_component_init_query(bool enable_progress_threads,
                                      bool enable_mpi_threads) 
{
   return OMPI_SUCCESS;
}

struct mca_fbtl_base_module_1_0_0_t *
mca_fbtl_gds_component_file_query (ompio_file_t *file, int *priority)
{
   *priority = mca_fbtl_gds_priority;

    if (GDS == file->f_fstype) {
        *priority = 50;
        return &gds;
    }

   return NULL;
}

int mca_fbtl_gds_component_file_unquery (ompio_file_t *file)
{
   return OMPI_SUCCESS;
}

int mca_fbtl_gds_module_init (ompio_file_t *file)
{
    int ret = mca_fbtl_base_get_fbtl_module (file, mca_fbtl_gds_cpumem_fbtl_name, &mca_fbtl_gds_cpumem_fbtl);

    if (OMPI_SUCCESS != ret) {
        opal_output(1, "mca_fbtl_gds_module_init: error in initializing fbtl component (%s) for cpu memory.\n" 
                       "\tPlease set --mca fbtl gds,%s",
                       mca_fbtl_gds_cpumem_fbtl_name, mca_fbtl_gds_cpumem_fbtl_name);
    }
    return ret;
}


int mca_fbtl_gds_module_finalize (ompio_file_t *file)
{
    return OMPI_SUCCESS;
}
