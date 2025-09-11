As per Stony Brook University Course CSE 220:

This project works "with the real, poorly-documented communication protocol used by PCIe called Transaction Layer Packets (TLP). The protocol is intended for transmitting blocks of 32-bit data between two different devices (this is how your CPU communicates with [NVMe] disk drives). One device, called the Host, is responsible for issuing all the read or write transactions. The other device, called the Endpoint is the location of the memory that is being written to or read from. There are many types of TLPs supported by the full PCIe protocol. This homework will only involve using 3 of them: Memory Write Request (32-bit addressing), Memory Read Request (32-bit addressing), and Completion with Data (this is the response to a read request that includes the data that has been read). 

Write Requests are the way for the Host to write data to the Endpoint’s memory across the bus. These packets need to include the memory address to be written to, the data to be written, how much data to expect, a “Byte-Enable” field that can be used if you are writing a smaller amount of data than the default word size of your system, and a Tag to indicate when the write has been finished.

Read Requests are the way that the Host can read data from the Endpoint across the bus. 

Completion is the mechanism that allows the Endpoint to provide the read data to the Host."
