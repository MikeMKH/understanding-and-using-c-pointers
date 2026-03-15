#pragma once

// Returns a heap-allocated string "Hello <name>!"
// Caller is responsible for freeing the returned pointer.
char *hello(const char *name);