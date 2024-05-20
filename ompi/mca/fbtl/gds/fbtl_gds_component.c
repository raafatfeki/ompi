/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2008-2020 University of Houston. All rights reserved.
 * Copyright (c) 2015      Los Alamos National Security, LLC. All rights
 *                         reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 * These symbols are in a file by themselves to provide nice linker
 * semantics.  Since linkers generally pull in symbols by object
 * files, keeping these symbols as the only symbols in this file
 * prevents utility programs such as "ompi_info" from having to import
 * entire components just to query their version and parameters.
 */

#include "ompi_config.h"
#include "fbtl_gds.h"
#include "mpi.h"

/*
 * Public string showing the fbtl gds component version number
 */
const char *mca_fbtl_gds_component_version_string =
  "OMPI/MPI gds FBTL MCA component version " OMPI_VERSION;

int mca_fbtl_gds_priority = 0;
int mca_fbtl_gds_directio_block_size = 4096;
const char* mca_fbtl_gds_cpumem_fbtl_name = "posix";
mca_fbtl_base_module_t *mca_fbtl_gds_cpumem_fbtl = NULL;

static int register_component(void);

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */
mca_fbtl_base_component_2_0_0_t mca_fbtl_gds_component = {

    /* First, the mca_component_t struct containing meta information
       about the component itself */

    .fbtlm_version = {
        MCA_FBTL_BASE_VERSION_2_0_0,

        /* Component name and version */
        .mca_component_name = "gds",
        MCA_BASE_MAKE_VERSION(component, OMPI_MAJOR_VERSION, OMPI_MINOR_VERSION,
                              OMPI_RELEASE_VERSION),
        .mca_register_component_params = register_component,
    },
    .fbtlm_data = {
        /* This component is checkpointable */
      MCA_BASE_METADATA_PARAM_CHECKPOINT
    },
    .fbtlm_init_query = mca_fbtl_gds_component_init_query,      /* get thread level */
    .fbtlm_file_query = mca_fbtl_gds_component_file_query,      /* get priority and actions */
    .fbtlm_file_unquery = mca_fbtl_gds_component_file_unquery,  /* undo what was done by previous function */
};


static int register_component(void)
{
    return OMPI_SUCCESS;
}

