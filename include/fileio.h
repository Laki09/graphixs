//
//  fileio.h
//  graphixs
//
//  Created by Lars Höll on 24.08.25.
//
#pragma once
#ifndef FILEIO_H
#define FILEIO_H


#include <stddef.h>

char* read_file(const char* path);

void write_file(const char* path, const char* string);

#endif
