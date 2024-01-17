#!/usr/bin/env python3
###############################################################################
# Top contributors (to current version):
#   Daniel Larraz
#
# This file is part of the cvc5 project.
#
# Copyright (c) 2009-2023 by the authors listed in the file AUTHORS
# in the top-level source directory and their institutional affiliations.
# All rights reserved.  See the file COPYING in the top-level source
# directory for licensing information.
# #############################################################################
#
# A simple test file to run cvc5 piping from stdin in non-interactive mode
##
import subprocess

def main():
    subprocess.check_output("echo '(set-logic ALL)' | bin/cvc5", shell=True)

if __name__ == "__main__":
    main()