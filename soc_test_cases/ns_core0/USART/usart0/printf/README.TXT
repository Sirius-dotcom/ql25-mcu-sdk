/*!
    \file  README.TXT
    \brief description of the USART printf test
    \version 2022-5-17, V1.0.1
*/

Function description:
This demo demonstrates USART0 printf functionality by printing processor MISA (Machine ISA) information.

Test procedure:
1. The test reads the MISA CSR register to determine processor capabilities
2. Constructs a string showing the RISC-V ISA extensions supported
3. Prints the MISA information in format: "MISA: RV<extensions>"
4. Calls simulation_pass() to indicate successful test completion

Expected output:
- Prints "MISA: RV<ISA_string>" where <ISA_string> contains:
  - Base ISA: "32", "64", or "128" 
  - Supported extensions: I, M, A, B, C, D, E, F, H, J, L, N, P, Q, S, T, U, X

Test result:
If it prints "MISA: RV<extensions>" and completes without errors, then pass.
