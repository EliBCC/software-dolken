!!! warning "These module docs are in beta and may be incomplete."

# modm:platform:core: ARM Cortex-M Core

This module generates the startup code, the vector table, the linkerscript and
provides runtime support for dynamic memory allocations on the heap and
assertion handling.

## Linkerscript

The linkerscript is generated for the devices memory map.

### Placement of `.fastcode` section

From the Cortex-M3 Technical Reference Manual:

> 14.5 System Interface:
>
> The system interface is a 32-bit AHB-Lite bus. Instruction and vector fetches,
> and data and debug accesses to the System memory space, 0x20000000 - 0xDFFFFFFF,
> 0xE0100000 - 0xFFFFFFFF, are performed over this bus.
>
> 14.5.6 Pipelined instruction fetches:
>
> To provide a clean timing interface on the System bus, instruction and vector
> fetch requests to this bus are registered. This results in an additional cycle
> of latency because instructions fetched from the System bus take two cycles.
> This also means that back-to-back instruction fetches from the System bus are
> not possible.
>
> Note:
>   Instruction fetch requests to the ICode bus are not registered.
>   Performance critical code must run from the ICode interface.

So for STM32s where the CCM is _not_ connected to the I-Bus, we execute
`.fastcode` from Flash.


## Options

#### stack_execution_guard

Fill the stack with relative jumps to fault handler to prevent accidental execution.: `False` ∈ `{ True, [4mFalse[24m[0m }`



#### allocator

Dynamic memory allocation strategy: `newlib` ∈ `{ block, [4mnewlib[24m, tlsf[0m }`

By default, the arm-none-eabi toolchain ships with the `newlib` libc, which uses
`dlmalloc` as the underlying allocator algorithm and only requires the
implementation of the `void * sbrk(ptrdiff_t size)` hook. However, this limits
the allocator to use just _one_ memory region, which must then also be of
_continuous_ extend, since `sbrk` can only grow and shrink, but not jump.
Therefore, when using the `newlib` strategy, only the largest memory region is
used as heap! Depending on the device memory architecture this can leave large
memory regions unused.

For devices with very small memories, we recommend using the block allocator
strategy, which uses a very light-weight and simple algorithm. This also only
operates on one continuous memory region as heap.

!!! note
    Memories can have different traits, such as DMA-ability or access time. The
    default memory allocator functions (malloc, new, etc) only return DMA-able
    memories, ordered by fastest access time. Similarly the search for the
    largest memory region only considers DMA-able memory.

!!! warning
    For devices which contain separate memories laid out in a continuous way
    (often called SRAM1, SRAM2, etc.) the `newlib` and `block` strategies choose
    the largest continuous memory region, *even though* unaligned accesses
    across memory regions may not be supported in hardware and lead to a bus
    fault! Consider using the TLSF implementation, which does not suffer from
    this issue.

To use all non-statically allocated memory for heap, use the TLSF strategy,
which natively supports multiple memory regions. Our implementation treats
all internal memories as separate regions, so unaligned accesses across memory
boundaries are not an issue. To request heap memory of different traits, see
`modm::MemoryTraits`.

!!! note
    The TLSF implementation has a static overhead of about 1kB per memory trait
    group, however, these can then contain multiple non-continuous memory
    regions. The upside of this large static allocation is very fast allocation
    times of O(1), but we recommend using TLSF only for devices with multiple
    memory regions.

#### vector_table_location

Vector table location in ROM or RAM: `rom` ∈ `{ ram, [4mrom[24m[0m }`

The vector table is always stored in ROM and copied to RAM by the startup script
if required. You can modify the RAM vector table using the CMSIS NVIC functions:

- `void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)`
- `uint32_t NVIC_GetVector(IRQn_Type IRQn)`

For applications that do not modify the vector table at runtime, relocation to
RAM is not necessary and can save a few hundred bytes of static memory.

By default, the fastest option is chosen depending on the target memory
architecture. This does not always mean the table is copied into RAM, and
therefore may not be modifiable with this option!

From the ARM Cortex-M4 Technical Reference Manual on exception handling:

> - Processor state is automatically stored to the stack on an exception, and
>   automatically restored from the stack at the end of the Interrupt Service
>   Routine.
> - The vector is fetched in parallel to the state saving, enabling efficient
>   interrupt entry.

!!! warning
    Placing main stack and vector table into the same memory can significantly
    slow down interrupt latency, since both I-Code and D-Code memory interface
    need to fetch from the same access port.

#### main_stack_size

Minimum size of the application main stack: `3040` ∈ `{ 256 .. [4m3040[24m .. 65536[0m }`

The ARM Cortex-M uses a descending stack mechanism which is placed so that it
grows towards the beginning of RAM. In case of a stack overflow the hardware
then attempts to stack into invalid memory which triggers a HardFault. A stack
overflow will therefore never overwrite any static or heap memory and this
protection works without the MPU and therefore also on ARM Cortex-M0 devices.

If you enable either the LED or the logging HardFault option, a smaller stack
is added above the main stack. This stack is only used by the HardFault handler
when not enough memory remains in the main stack to preserve GDB backtrace
behavior. This memory also acts as a small safety buffer against main stack
underflow, which is not detected however.

If the vector table is relocated into RAM, the start address needs to be aligned
to the next highest power-of-two word depending on the total number of device
interrupts. On devices where the table is relocated into the same memory as the
main stack, an alignment buffer up to 1kB is added to the main stack.

```
|              ...                |
|---------------------------------|
|    Interrupt Vectors (in RAM)   |
|        (if re-mapped)           | <-- vector table origin
|---------------------------------| <-- HardFault stack top
|        HardFault Stack          |
|       (grows downwards)         |
|               |                 |
|               v                 |
|---------------------------------| <-- main stack top
|           Main Stack            |
|       (grows downwards)         |
|               |                 |
|               v                 |
|---------------------------------|
|  Alignment buffer for vectors   |
|   (overwritten by main stack!)  |
'---------------------------------' <-- RAM origin
```

!!! warning
    The main stack size you provide is a minimum and may be enlarged to satisfy
    alignment requirements. Be aware that these requirements operate on the sum
    of HardFault and main stack. Disabling HardFault options may therefore
    decrease the alignment buffer added to the main stack size, which may make
    your application overflow stack. You need to increase your minimum main
    stack size in that case.

!!! note
    The main stack is watermarked and you can get the maximum stack usage using
    the `uint32_t modm::cortex::getMaximumStackUsage()` function.

#### linkerscript.memory

Values: `{ String[0m }`



#### linkerscript.sections

Values: `{ String[0m }`



#### linkerscript.table_extern.zero

Values: `{ String[0m }`



#### linkerscript.table_extern.copy

Values: `{ String[0m }`



#### linkerscript.table_extern.heap

Values: `{ String[0m }`








## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.38.0 (20140413.2041)
 -->
<!-- Title: modm:platform:core Pages: 1 -->
<svg width="1168pt" height="150pt"
 viewBox="0.00 0.00 1168.00 150.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 146)">
<title>modm:platform:core</title>
<polygon fill="white" stroke="none" points="-4,4 -4,-146 1164,-146 1164,4 -4,4"/>
<!-- modm_platform_core -->
<g id="node1" class="node"><title>modm_platform_core</title>
<polygon fill="lightgrey" stroke="black" stroke-width="2" points="636.5,-53 569.5,-53 569.5,-0 636.5,-0 636.5,-53"/>
<text text-anchor="middle" x="603" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="603" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">platform:</text>
<text text-anchor="middle" x="603" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">core</text>
</g>
<!-- modm_architecture_accessor -->
<g id="node2" class="node"><title>modm_architecture_accessor</title>
<g id="a_node2"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:
architecture:
accessor">
<polygon fill="lightgrey" stroke="black" points="84,-142 0,-142 0,-89 84,-89 84,-142"/>
<text text-anchor="middle" x="42" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="42" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="42" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">accessor</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_accessor -->
<g id="edge1" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_accessor</title>
<path fill="none" stroke="black" d="M569.381,-27.6107C488.016,-28.7024 274.497,-36.504 93.7939,-89.205"/>
<polygon fill="black" stroke="black" points="92.7156,-85.8741 84.1266,-92.0773 94.7093,-92.5842 92.7156,-85.8741"/>
</g>
<!-- modm_architecture_assert -->
<g id="node3" class="node"><title>modm_architecture_assert</title>
<g id="a_node3"><a xlink:href="../modm-architecture-assert" xlink:title="modm:
architecture:
assert">
<polygon fill="lightgrey" stroke="black" points="186,-142 102,-142 102,-89 186,-89 186,-142"/>
<text text-anchor="middle" x="144" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="144" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="144" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">assert</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_assert -->
<g id="edge2" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_assert</title>
<path fill="none" stroke="black" d="M569.413,-29.0066C500.211,-32.8585 337.082,-45.8044 195.856,-89.1421"/>
<polygon fill="black" stroke="black" points="194.656,-85.8505 186.158,-92.1778 196.747,-92.5309 194.656,-85.8505"/>
</g>
<!-- modm_architecture_atomic -->
<g id="node4" class="node"><title>modm_architecture_atomic</title>
<g id="a_node4"><a xlink:href="../modm-architecture-atomic" xlink:title="modm:
architecture:
atomic">
<polygon fill="lightgrey" stroke="black" points="288,-142 204,-142 204,-89 288,-89 288,-142"/>
<text text-anchor="middle" x="246" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="246" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="246" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">atomic</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_atomic -->
<g id="edge3" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_atomic</title>
<path fill="none" stroke="black" d="M569.179,-31.318C513.668,-38.2175 399.985,-55.0113 297.751,-89.0991"/>
<polygon fill="black" stroke="black" points="296.553,-85.8096 288.212,-92.3424 298.806,-92.437 296.553,-85.8096"/>
</g>
<!-- modm_architecture_clock -->
<g id="node5" class="node"><title>modm_architecture_clock</title>
<g id="a_node5"><a xlink:href="../modm-architecture-clock" xlink:title="modm:
architecture:
clock">
<polygon fill="lightgrey" stroke="black" points="390,-142 306,-142 306,-89 390,-89 390,-142"/>
<text text-anchor="middle" x="348" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="348" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="348" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">clock</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_clock -->
<g id="edge4" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_clock</title>
<path fill="none" stroke="black" d="M569.187,-35.7964C530.144,-45.7864 464.011,-63.97 399.543,-89.0347"/>
<polygon fill="black" stroke="black" points="398.064,-85.8557 390.047,-92.7825 400.634,-92.3669 398.064,-85.8557"/>
</g>
<!-- modm_architecture_delay -->
<g id="node6" class="node"><title>modm_architecture_delay</title>
<g id="a_node6"><a xlink:href="../modm-architecture-delay" xlink:title="modm:
architecture:
delay">
<polygon fill="lightgrey" stroke="black" points="492,-142 408,-142 408,-89 492,-89 492,-142"/>
<text text-anchor="middle" x="450" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="450" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="450" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">delay</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_delay -->
<g id="edge5" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_delay</title>
<path fill="none" stroke="black" d="M569.424,-46.5925C549.388,-57.9852 523.545,-72.6803 501.026,-85.4851"/>
<polygon fill="black" stroke="black" points="499.219,-82.4866 492.256,-90.4722 502.679,-88.5717 499.219,-82.4866"/>
</g>
<!-- modm_architecture_heap -->
<g id="node7" class="node"><title>modm_architecture_heap</title>
<g id="a_node7"><a xlink:href="../modm-architecture-heap" xlink:title="modm:
architecture:
heap">
<polygon fill="lightgrey" stroke="black" points="594,-142 510,-142 510,-89 594,-89 594,-142"/>
<text text-anchor="middle" x="552" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="552" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="552" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">heap</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_heap -->
<g id="edge6" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_heap</title>
<path fill="none" stroke="black" d="M587.974,-53.1323C583.038,-61.5529 577.468,-71.0547 572.211,-80.0216"/>
<polygon fill="black" stroke="black" points="569.132,-78.3532 567.095,-88.7503 575.171,-81.8933 569.132,-78.3532"/>
</g>
<!-- modm_architecture_interrupt -->
<g id="node8" class="node"><title>modm_architecture_interrupt</title>
<g id="a_node8"><a xlink:href="../modm-architecture-interrupt" xlink:title="modm:
architecture:
interrupt">
<polygon fill="lightgrey" stroke="black" points="696,-142 612,-142 612,-89 696,-89 696,-142"/>
<text text-anchor="middle" x="654" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="654" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="654" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">interrupt</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_interrupt -->
<g id="edge7" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_interrupt</title>
<path fill="none" stroke="black" d="M618.026,-53.1323C622.962,-61.5529 628.532,-71.0547 633.789,-80.0216"/>
<polygon fill="black" stroke="black" points="630.829,-81.8933 638.905,-88.7503 636.868,-78.3532 630.829,-81.8933"/>
</g>
<!-- modm_architecture_memory -->
<g id="node9" class="node"><title>modm_architecture_memory</title>
<g id="a_node9"><a xlink:href="../modm-architecture-memory" xlink:title="modm:
architecture:
memory">
<polygon fill="lightgrey" stroke="black" points="798,-142 714,-142 714,-89 798,-89 798,-142"/>
<text text-anchor="middle" x="756" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="756" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="756" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">memory</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_memory -->
<g id="edge8" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_memory</title>
<path fill="none" stroke="black" d="M636.576,-46.5925C656.612,-57.9852 682.455,-72.6803 704.974,-85.4851"/>
<polygon fill="black" stroke="black" points="703.321,-88.5717 713.744,-90.4722 706.781,-82.4866 703.321,-88.5717"/>
</g>
<!-- modm_architecture_unaligned -->
<g id="node10" class="node"><title>modm_architecture_unaligned</title>
<g id="a_node10"><a xlink:href="../modm-architecture-unaligned" xlink:title="modm:
architecture:
unaligned">
<polygon fill="lightgrey" stroke="black" points="900,-142 816,-142 816,-89 900,-89 900,-142"/>
<text text-anchor="middle" x="858" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="858" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="858" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">unaligned</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_architecture_unaligned -->
<g id="edge9" class="edge"><title>modm_platform_core&#45;&gt;modm_architecture_unaligned</title>
<path fill="none" stroke="black" d="M636.813,-35.7964C675.856,-45.7864 741.989,-63.97 806.457,-89.0347"/>
<polygon fill="black" stroke="black" points="805.366,-92.3669 815.953,-92.7825 807.936,-85.8557 805.366,-92.3669"/>
</g>
<!-- modm_cmsis_device -->
<g id="node11" class="node"><title>modm_cmsis_device</title>
<g id="a_node11"><a xlink:href="../modm-cmsis-device" xlink:title="modm:
cmsis:
device">
<polygon fill="lightgrey" stroke="black" points="974,-142 918,-142 918,-89 974,-89 974,-142"/>
<text text-anchor="middle" x="946" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="946" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">cmsis:</text>
<text text-anchor="middle" x="946" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">device</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_cmsis_device -->
<g id="edge10" class="edge"><title>modm_platform_core&#45;&gt;modm_cmsis_device</title>
<path fill="none" stroke="black" d="M636.909,-29.8372C694.787,-34.7908 815.454,-49.3517 909,-89 909.094,-89.04 909.189,-89.0802 909.283,-89.1207"/>
<polygon fill="black" stroke="black" points="907.481,-92.1317 917.985,-93.5144 910.636,-85.883 907.481,-92.1317"/>
</g>
<!-- modm_platform -->
<g id="node12" class="node"><title>modm_platform</title>
<g id="a_node12"><a xlink:href="../modm-platform" xlink:title="modm:
platform">
<polygon fill="lightgrey" stroke="black" points="1056,-134.5 992,-134.5 992,-96.5 1056,-96.5 1056,-134.5"/>
<text text-anchor="middle" x="1024" y="-119.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="1024" y="-104.3" font-family="Times New Roman,serif" font-size="14.00">platform</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_platform -->
<g id="edge11" class="edge"><title>modm_platform_core&#45;&gt;modm_platform</title>
<path fill="none" stroke="black" d="M636.658,-28.5244C704.359,-31.53 860.962,-43.2836 983,-89 985.105,-89.7884 987.218,-90.6915 989.316,-91.6764"/>
<polygon fill="black" stroke="black" points="987.852,-94.861 998.332,-96.418 991.11,-88.6656 987.852,-94.861"/>
</g>
<!-- modm_platform_clock_cortex -->
<g id="node13" class="node"><title>modm_platform_clock_cortex</title>
<g id="a_node13"><a xlink:href="../modm-platform-clock-cortex" xlink:title="modm:
platform:
clock.cortex">
<polygon fill="lightgrey" stroke="black" points="1160,-142 1074,-142 1074,-89 1160,-89 1160,-142"/>
<text text-anchor="middle" x="1117" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="1117" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">platform:</text>
<text text-anchor="middle" x="1117" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">clock.cortex</text>
</a>
</g>
</g>
<!-- modm_platform_core&#45;&gt;modm_platform_clock_cortex -->
<g id="edge12" class="edge"><title>modm_platform_core&#45;&gt;modm_platform_clock_cortex</title>
<path fill="none" stroke="black" d="M636.546,-28.2768C712.286,-30.8039 901.936,-41.479 1064.07,-89.1217"/>
<polygon fill="black" stroke="black" points="1063.14,-92.4972 1073.72,-92.0131 1065.15,-85.7914 1063.14,-92.4972"/>
</g>
</g>
</svg>

