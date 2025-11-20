#ifndef PRINTER_H
#define PRINTER_H

#include "ttps.h"
#include <string>
using namespace std;

void printTree(Node* root, const string& prefix = "", bool isLast = true);

#endif
