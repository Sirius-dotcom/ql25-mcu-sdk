.. _overview:

Overview
========

.. _overview_intro:

Introduction
------------

The **Nuclei SoC Software Development Kit (SDK)** is an customized SoC SDK to
speed up the software development of SoCs based on Nuclei Processor Cores.

This Nuclei SoC SDK is built based on the |nuclei_sdk|, user can benefit from
the **Build System** and **NMSIS API** provided by Nuclei SDK .

.. _overview_design_arch:

Design and Architecture
-----------------------

The Nuclei SoC SDK general design and architecture are shown in the block diagram as below.

.. _figure_overview_1:

.. figure:: /asserts/images/nuclei_soc_sdk_diagram.png
   :width: 80 %
   :align: center
   :alt: Nuclei SoC SDK Design and Architecture Diagram

   Nuclei SoC SDK Design and Architecture Diagram

As :ref:`figure_overview_1` shown, The Nuclei SoC SDK provides the following features:

* Nuclei Core API service is built on top of |NMSIS|, so silicon vendors of Nuclei processors can easily
  port their SoCs to Nuclei SDK, and quickly evaluate software on their SoC.
* **NMSIS-NN** and **NMSIS-DSP** library can be also used in Nuclei SDK, and the prebuilt libraries are
  included in **NMSIS/Library** folder of Nuclei SoC SDK.
* Mainly support customized SoC based on Nuclei Processor Core.
* Provided realtime operation system service based on Nuclei SDK.
* Provided bare-metal service for embedded system software beginners and resource-limited use-cases.

.. _overview_getstarted:

Get Started
-----------

Please refer to :ref:`quickstart` to get started to take a try with Nuclei SoC SDK.

.. _overview_copyright:

Copyright
---------

Copyright (c) 2019 - Present, Nuclei System Technology. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the Nuclei System Technology., nor the names of its contributors
   may be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. NY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

.. _overview_license:

License
-------

Nuclei SoC SDK is licensed by :ref:`Apache License 2.0 <license>`.
