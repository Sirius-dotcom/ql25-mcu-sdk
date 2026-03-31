.. _quickstart:

Quick Startup
=============

Setup Tools and Environment
---------------------------

To start to use Nuclei SoC SDK, you need to install the following tools provided by Nuclei:

Click https://doc.nucleisys.com/nuclei_sdk/quickstart.html#setup-tools-and-environment to learn
about how to setup tools.

.. _quickstart_buildrundebug_app:

Build, Run and Debug Sample Application
---------------------------------------

Assume you have setup toolchain and openocd as above and create ``setup_config.bat``
or ``setup_config.sh`` according to your development environment in your SoC SDK.

To build, run and debug application, you need to open command terminal in your sdk folder,
assume it to be ``soc_sdk``.

Here for a quick startup, this guide will take your customized development board
for example to demostrate how to setup hardware, build run and debug application in Windows.

The demo application, we will take ``application/baremetal/helloworld`` for example.

First of all, please open command terminal in ``soc_sdk`` folder, and run following command
to setup environment.

* In Linux Bash: ``source setup.sh``
* In Windows cmd: ``setup.bat``

Run ``cd application/baremetal/helloworld`` to cd the ``helloworld`` example folder.

Hardware Preparation
~~~~~~~~~~~~~~~~~~~~

Please follow the Board and SoC user manual to learn about how to setup your hardware,
mainly **JTAG debugger driver setup and on-board connection setup**.

* Power on the board, and use JTAG to connect the board and your PC,
  make sure you have setup the JTAG driver correctly, and you can see JTAG port and serial port.
* Open a UART terminal tool such as |teraterm| or |minicom|, and minitor the serial port of the Board,
  the UART baudrate is *115200 bps*

Build Application
~~~~~~~~~~~~~~~~~

.. note::

    Since in this SoC SDK, the **SOC** and **BOARD** variables are adapted to match your board,
    so there is no need to pass **SOC** and **BOARD** variables in make command.

We need to build application for this board using this command line:

.. code-block:: shell

    make all

As you can see, that when the application is built successfully, the elf will be generated
and will also print the size information of the ``hello_world.elf``.

.. note::

    * In order to make sure that there is no application build before, you can run
      ``make clean`` to clean previously built objects and build dependency files.

Run Application
~~~~~~~~~~~~~~~

If the application is built successfully for this board then you can upload application to board
using this command line:

.. code-block:: shell

    make upload

As you can see the application is uploaded successfully using ``openocd`` and ``gdb``,
then you can check the output in your UART terminal


Debug Application
~~~~~~~~~~~~~~~~~

If the application is built successfully for this board then you can debug it using this command line:

.. code-block:: shell

    make debug


1. The program is not loaded automatically when you enter to debug state, just in case you want to
   debug the program running on the board.


2. If you want to load the built application, you can type ``load`` to load the application.

   .. code-block::

      (gdb) load
      Loading section .init, size 0x266 lma 0x8000000
      Loading section .text, size 0x2e9c lma 0x8000280
      Loading section .rodata, size 0x1f0 lma 0x8003120
      Loading section .data, size 0x70 lma 0x8003310
      Start address 0x800015c, load size 13154
      Transfer rate: 7 KB/sec, 3288 bytes/write.

3. If you want to set a breakpoint at *main*, then you can type ``b main`` to set a breakpoint.

   .. code-block::

      (gdb) b main
      Breakpoint 1 at 0x8001b04: file hello_world.c, line 85.

4. If you want to set more breakpoints, you can do as you like.

5. Then you can type ``c``, then the program will stop at **main**

   .. code-block::

      (gdb) c
      Continuing.
      Note: automatically using hardware breakpoints for read-only addresses.

      Breakpoint 1, main () at hello_world.c:85
      85          srand(__get_rv_cycle()  | __get_rv_instret() | __RV_CSR_READ(CSR_MCYCLE));


6. Then you can step it using ``n`` (short of next) or ``s`` (short of step)

   .. code-block::

      (gdb) n
      86          uint32_t rval = rand();
      (gdb) n
      87          rv_csr_t misa = __RV_CSR_READ(CSR_MISA);
      (gdb) s
      89          printf("MISA: 0x%lx\r\n", misa);
      (gdb) n
      90          print_misa();
      (gdb) n
      92          printf("Hello World!\r\n");
      (gdb) n
      93          printf("Hello World!\r\n");

7. If you want to quit debugging, then you can press ``CTRL - c``, and type ``q`` to quit debugging.

   .. code-block::

      (gdb) Quit
      (gdb) q
      A debugging session is active.

              Inferior 1 [Remote target] will be detached.

      Quit anyway? (y or n) y
      Detaching from program: D:\workspace\Sourcecode\soc_sdk\application\baremetal\helloworld\hello_world.elf, Remote target
      Ending remote debugging.
      [Inferior 1 (Remote target) detached]

.. note::

   * More about how to debug using gdb, you can refer to the `GDB User Manual`_.
   * For more application usage, please check https://doc.nucleisys.com/nuclei_sdk/design/app.html
   * To learn about SDK Build System usage, please check https://doc.nucleisys.com/nuclei_sdk/develop/index.html

.. _GDB User Manual: https://www.gnu.org/software/gdb/documentation/
