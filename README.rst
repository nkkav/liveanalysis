==========================
 liveanalysis user manual
==========================

+-------------------+----------------------------------------------------------+
| **Title**         | liveanalysis (Liveness analysis pass for Machine-SUIF)   |
+-------------------+----------------------------------------------------------+
| **Author**        | Nikolaos Kavvadias 2004, 2005, 2006, 2007, 2008, 2009    |
|                   | 2010, 2011, 2012, 2013, 2014                             |
+-------------------+----------------------------------------------------------+
| **Contact**       | nikos@nkavvadias.com                                     |
+-------------------+----------------------------------------------------------+
| **Website**       | http://www.nkavvadias.com                                |
+-------------------+----------------------------------------------------------+
| **Release Date**  | 01 October 2014                                          |
+-------------------+----------------------------------------------------------+
| **Version**       | 1.0.0                                                    |
+-------------------+----------------------------------------------------------+
| **Rev. history**  |                                                          |
+-------------------+----------------------------------------------------------+
|        **v1.0.0** | 2004-10-01                                               |
|                   |                                                          |
|                   | Initial release.                                         |
+-------------------+----------------------------------------------------------+


1. Introduction
===============

``liveanalysis`` is a liveness analysis pass built to be used with the SUIF2/
MachSUIF2 compiler infrastructure. This pass generates a textual representation 
for the liveness information in a given control-flow graph. It reports the 
live-in and live-out sets, while the kill and gen set can also be reported by 
setting the ``REPORT_KILL_GEN_SET`` compile-time switch.

This pass uses the cfa, bvd and machine libraries of Machine-SUIF. It works for 
the SUIFvm instruction set as well as other MachSUIF backends. The 
``liveanalysis`` pass has been tested with MachSUIF 2.02.07.15.


2. File listing
===============

The ``liveanalysis`` distribution includes the following files:
   
+-----------------------+------------------------------------------------------+
| /liveanalysis         | Top-level directory                                  |
+-----------------------+------------------------------------------------------+
| AUTHORS               | List of ``liveanalysis`` authors.                    |
+-----------------------+------------------------------------------------------+
| LICENSE               | The modified BSD license governs ``liveanalysis``.   |
+-----------------------+------------------------------------------------------+
| README.rst            | This file.                                           |
+-----------------------+------------------------------------------------------+
| README.html           | HTML version of README.                              |
+-----------------------+------------------------------------------------------+
| README.pdf            | PDF version of README.                               |
+-----------------------+------------------------------------------------------+
| VERSION               | Current version of the project sources.              |
+-----------------------+------------------------------------------------------+
| liveanalysis.cpp      | Implementation of the ``liveanalysis`` pass.         |
+-----------------------+------------------------------------------------------+
| liveanalysis.h        | C++ header file containing declarations and          |
|                       | prototypes for the above.                            |
+-----------------------+------------------------------------------------------+
| rst2docs.sh           | Bash script for generating the HTML and PDF versions |
|                       | of the documentation (README).                       |
+-----------------------+------------------------------------------------------+
| suif_main.cpp         | Entry point for building the standalone program      |
|                       | ``do_liveanalysis`` that implements the pass.        |
+-----------------------+------------------------------------------------------+
| suif_pass.cpp         | Define the SUIF pass built as the dynamically        |
|                       | loadable library ``libliveanalysis.so``.             |
+-----------------------+------------------------------------------------------+
| suif_main.h           | C++ header file for the above.                       |
+-----------------------+------------------------------------------------------+


3. Installation
===============

Unpack the ``liveanalysis`` archive wherever you like, e.g. in 
``$MACHSUIFHOME/cfa/liveanalysis``. You don't need to modify anything in the 
Makefile, if you have a working MachSUIF 2 installation.

The program binary (``do_liveanalysis``) will be installed at ``$NCIHOME/bin`` 
and the shared library (``libliveanalysis.so``) at ``$NCIHOME/solib``, where 
NCIHOME is the SUIF 2 top-level directory.


4. Usage details
================

The pass accepts an input file in CFG form to operate. Textual output is 
generated, written to stdout by default.

Usage synopsys:
| ``$ do_liveanalysis test.cfg``
