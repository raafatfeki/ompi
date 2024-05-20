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
 * Copyright (c) 2018      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
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
#include "ompi/mca/fs/fs.h"
#include "ompi/mca/fs/base/base.h"
#include "ompi/mca/fs/gds/fs_gds.h"

/*
 * *******************************************************************
 * ************************ actions structure ************************
 * *******************************************************************
 */
static mca_fs_base_module_1_0_0_t gds =  {
	mca_fs_gds_module_init,
	mca_fs_gds_module_finalize,
	mca_fs_gds_file_open,
	mca_fs_gds_file_close,
	mca_fs_base_file_delete,
	mca_fs_base_file_set_size,
	mca_fs_base_file_get_size,
	mca_fs_base_file_sync
};
/*
 * *******************************************************************
 * ************************* structure ends **************************
 * *******************************************************************
 */

int mca_fs_gds_component_init_query(bool enable_progress_threads,
									  			bool enable_mpi_threads)
{
   return OMPI_SUCCESS;
}

struct mca_fs_base_module_1_0_0_t *
mca_fs_gds_component_file_query (ompio_file_t *file, int *priority)
{
	*priority = mca_fs_gds_priority;
	/* We suppose that fs gds is only selected by users.
	 * If it does not work properly, it shoudl fail and no other fs component is selected.
	 * Therefore, we call cuFileDriverOpen() in init not in query.
	 * PS: If we decide to check other component, we should move the code from init to query.
	 */
	return &gds;
}

int mca_fs_gds_component_file_unquery (ompio_file_t *file)
{
	return OMPI_SUCCESS;
}

int mca_fs_gds_module_init (ompio_file_t *file)
{
	/* When the gds API is called, it internally checks if it is supported or not, 
	therefore, I opened the cuFileDriver here, if it fails then it is not supported
	*/
	CUfileError_t status = cuFileDriverOpen();
	if (CU_FILE_SUCCESS != status.err) {
      CUFILE_ERROR_OUTPUT(1, "CUFILE driver open error", status);
		file->f_fstype = 0;
		return OMPI_ERROR;
	}

	file->f_fstype = GDS;
	return OMPI_SUCCESS;
}


int mca_fs_gds_module_finalize (ompio_file_t *file)
{
	CUfileError_t status = cuFileDriverClose();
	if (CU_FILE_SUCCESS != status.err) {
      CUFILE_ERROR_OUTPUT(1, "CUFILE driver close error", status);	
	}

	return OMPI_SUCCESS;
}
