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
 */

#ifndef MCA_FBTL_GDS_H
#define MCA_FBTL_GDS_H

#include "ompi_config.h"
#include "ompi/mca/mca.h"
#include "ompi/mca/fbtl/fbtl.h"
#include "ompi/mca/common/ompio/common_ompio.h"
#include "ompi/mca/common/ompio/common_ompio_request.h"

#include <cufile.h>

extern int mca_fbtl_gds_directio_block_size;
extern int mca_fbtl_gds_priority;
extern const char* mca_fbtl_gds_cpumem_fbtl_name;
extern mca_fbtl_base_module_t *mca_fbtl_gds_cpumem_fbtl;

BEGIN_C_DECLS

int mca_fbtl_gds_component_init_query(bool enable_progress_threads,
                                      bool enable_mpi_threads);
struct mca_fbtl_base_module_1_0_0_t *
	mca_fbtl_gds_component_file_query (ompio_file_t *file, int *priority);
int mca_fbtl_gds_component_file_unquery (ompio_file_t *file);
int mca_fbtl_gds_module_init (ompio_file_t *file);
int mca_fbtl_gds_module_finalize (ompio_file_t *file);

OMPI_DECLSPEC extern mca_fbtl_base_component_2_0_0_t mca_fbtl_gds_component;
/*
 * ******************************************************************
 * ********* functions which are implemented in this module *********
 * ******************************************************************
 */
ssize_t mca_fbtl_gds_preadv (ompio_file_t *file );
ssize_t mca_fbtl_gds_pwritev (ompio_file_t *file );
ssize_t mca_fbtl_gds_ipreadv (ompio_file_t *file, ompi_request_t *request);
ssize_t mca_fbtl_gds_ipwritev (ompio_file_t *file, ompi_request_t *request);

bool 	mca_fbtl_gds_progress     ( mca_ompio_request_t *req);
void 	mca_fbtl_gds_request_free ( mca_ompio_request_t *req);

/*
 * ******************************************************************
 * ************ functions implemented in this module end ************
 * ******************************************************************
 */

END_C_DECLS

#endif /* MCA_FBTL_GDS_H */
