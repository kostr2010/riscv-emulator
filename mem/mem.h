#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

// CSR and 32 basic registers are held in regfile

// we have 34 bit physical address space and 32 bit virtual address space
// simulator has 4mb of RAM

// physical memory layout
/*---------------------------------- 0x00000000
|   imitation of restricted zone
|----------------------------------- 0x0000000F
|               Text (ELF)
|-----------------------------------
|               Global data
|-----------------------------------
|            stack (grows down)
|                  |
|-----------------------------------
|                  |
|            Heap (grows up)
|----------------------------------- 0x003FFFFF
*/

// virtual memory layout
/*---------------------------------- 0x00000000
|       imitation of kernal space
|----------------------------------- 0x000000FF
|               Text (ELF)
|-----------------------------------
|               Global data
|-----------------------------------
|            stack (grows down)
|                  |
|-----------------------------------
|                  |
|            Heap (grows up)
|----------------------------------- 0x003FFFFF
*/

// translation rule: addr[p] = addr[v] - 0x000000F0

#endif