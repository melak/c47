// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: Copyright The WP43 Authors

/**
 * \file saveRestorePrograms.h
 */
#if !defined(SAVERESTOREPROGRAMS_H)
  #define SAVERESTOREPROGRAMS_H

  #include "typeDefinitions.h"
  #include <stdint.h>

  void fnSaveProgram (uint16_t label);
  void fnLoadProgram (uint16_t unusedButMandatoryParameter);

#endif // !SAVERESTOREPROGRAMS_H
