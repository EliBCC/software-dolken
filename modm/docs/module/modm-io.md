!!! warning "These module docs are in beta and may be incomplete."

# modm:io: Input/Output Streams

The `modm::IOStream` class contains efficient formatting that supports both
C++ `std::basic_ostream`-like formatting via `operator <<` as well as
implementing a reasonably large subset of `printf`.

```cpp
modm::IOStream stream(device);
stream << "format number 8: " << uint8_t(8) << " or as signed -100: " << int8_t(-100);
stream << modm::endl;

stream.printf("format number 8: %u or as signed -100: %d", 8, -100);
```

## Using printf

The format string is composed of zero or more directives: ordinary
characters (not %), which are copied unchanged to the output stream;
and conversion specifications, each of which results in fetching zero
or more subsequent arguments. Each conversion specification is
introduced by the % character. The arguments must properly correspond
(after type promotion) with the conversion specifier. After the %,
the following appear in sequence:

Zero or more of the following flags:

- `-` A negative field width flag; the converted value is to be
  left adjusted on the field boundary. The converted value is
  padded on the right with blanks, rather than on the left with
  blanks.
- `' '` (space) A blank should be left before a positive number
  produced by a signed conversion (d).
- `+` A sign must always be placed before a number produced by a
  signed conversion. A `+` overrides a space if both are used.
- An optional decimal digit string specifying a minimum field width.
  If the converted value has fewer characters than the field width,
  it will be padded with spaces on the left (or right, if the
  left-adjustment flag has been given) to fill out the field width.
- An optional `h`, `l` or `ll` length modifier, that specifies that the argument
  for the `d`, `u`, or `x` conversion is a 8-bit ("h"), 32-bit ("l") or
  64-bit ("ll") rather than 16-bit.

The conversion specifiers and their meanings are:

- `c`: char (8-bit)
- `s`: string (`char *`)
- `p`: pointer (`void *`)
- `d`: signed  decimal
- `u`: unsigned decimal
- `x`: hex
- `f`: float
- `%`: %

Combined with the length modifiers you get:

- `d`:    signed 16-bit
- `ld`:   signed 32-bit
- `lld`:  signed 64-bit (not yet)
- `u`:    unsigned 16-bit
- `lu`:   unsigned 32-bit
- `llu`:  unsigned 64-bit (not yet)
- `hx`:    8-bit hex
- `x`:    16-bit hex
- `lx`:   32-bit hex
- `llx`:  64-bit hex (not yet)

Examples, given `-100` as argument in the right type:

```
%c:
%s:
%p:  0x0100
%d:  -100
%ld: -100
%u:  65436
%lu:
%hx: 0x9c
%x:  0xff9c
%lx: 0xffffff9c
```

- `s`: The "char *" argument is expected to be a pointer to an array
  of character type (pointer to a string). Characters from the array
  are written up to (but not including) a terminating NULL character;
  if a precision is specified, no more than the number specified are
  written. If a precision is given, no null character need be present;
  if the precision is not specified, or is greater than the size of
  the array, the array must contain a terminating NULL character.
- `%`: A `%` is written. No argument is converted. The complete conversion
  specification is `%%`.

In no case does a non-existent or small field width cause truncation
of a numeric field; if the result of a conversion is wider than the
field width, the field is expanded to contain the conversion result.


## Redirecting IOStreams

The `modm::IODeviceWrapper` transforms any peripheral device that provides static
`write()` and `read()` functions into an `IODevice`.

You have to decide what happens when the device buffer is full and you cannot
write to it at the moment. There are two options:

1. busy wait until the buffer is free, or
2. discard the bytes that cannot be written.

Option 1 has the advantage, that none of your data will be lost,
however, busy-waiting can take a long time and can mess up your
program timings.
There is also a **high risk of deadlock**, when writing to a
IODevice inside of an interrupt and then busy-waiting forever
because the IODevice requires interrupts itself to send out
the data.

It is therefore highly recommended to use option 2, where surplus
data will be discarded.
You should increase the IODevice buffer size, if you experience
missing data from your connection.
This behavior is also deadlock safe when called from inside another
interrupt, and your program timing is minimally affected (essentially
only coping data into the buffer).

There is no default template argument, so that you hopefully make
a conscious decision and be aware of this behavior.

Example:

```cpp
// configure a UART
using Uart = Uart0;

// wrap it into an IODevice
modm::IODeviceWrapper<Uart, modm::IOBuffer::DiscardIfFull> device;

// use this device to print a message
device.write("Hello");

// or create a IOStream and use the stream to print something
modm::IOStream stream(device);
stream << " World!";
```






## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.38.0 (20140413.2041)
 -->
<!-- Title: modm:io Pages: 1 -->
<svg width="554pt" height="224pt"
 viewBox="0.00 0.00 554.00 224.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 220)">
<title>modm:io</title>
<polygon fill="white" stroke="none" points="-4,4 -4,-220 550,-220 550,4 -4,4"/>
<!-- modm_io -->
<g id="node1" class="node"><title>modm_io</title>
<polygon fill="lightgrey" stroke="black" stroke-width="2" points="321,-127 265,-127 265,-89 321,-89 321,-127"/>
<text text-anchor="middle" x="293" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="293" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">io</text>
</g>
<!-- modm_architecture_accessor -->
<g id="node2" class="node"><title>modm_architecture_accessor</title>
<g id="a_node2"><a xlink:href="../modm-architecture-accessor" xlink:title="modm:
architecture:
accessor">
<polygon fill="lightgrey" stroke="black" points="291,-216 207,-216 207,-163 291,-163 291,-216"/>
<text text-anchor="middle" x="249" y="-200.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="249" y="-185.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="249" y="-170.8" font-family="Times New Roman,serif" font-size="14.00">accessor</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_architecture_accessor -->
<g id="edge1" class="edge"><title>modm_io&#45;&gt;modm_architecture_accessor</title>
<path fill="none" stroke="black" d="M277.255,-127.04C272.034,-135.011 266.379,-144.663 261.433,-153.932"/>
<polygon fill="black" stroke="black" points="258.232,-152.504 256.766,-162.997 264.455,-155.708 258.232,-152.504"/>
</g>
<!-- modm_math_utils -->
<g id="node3" class="node"><title>modm_math_utils</title>
<g id="a_node3"><a xlink:href="../modm-math-utils" xlink:title="modm:
math:
utils">
<polygon fill="lightgrey" stroke="black" points="365,-216 309,-216 309,-163 365,-163 365,-216"/>
<text text-anchor="middle" x="337" y="-200.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="337" y="-185.8" font-family="Times New Roman,serif" font-size="14.00">math:</text>
<text text-anchor="middle" x="337" y="-170.8" font-family="Times New Roman,serif" font-size="14.00">utils</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_math_utils -->
<g id="edge2" class="edge"><title>modm_io&#45;&gt;modm_math_utils</title>
<path fill="none" stroke="black" d="M302.984,-127.04C307.442,-135.094 312.849,-144.864 318.028,-154.221"/>
<polygon fill="black" stroke="black" points="314.98,-155.943 322.885,-162.997 321.105,-152.553 314.98,-155.943"/>
</g>
<!-- modm_utils -->
<g id="node4" class="node"><title>modm_utils</title>
<g id="a_node4"><a xlink:href="../modm-utils" xlink:title="modm:
utils">
<polygon fill="lightgrey" stroke="black" points="439,-208.5 383,-208.5 383,-170.5 439,-170.5 439,-208.5"/>
<text text-anchor="middle" x="411" y="-193.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="411" y="-178.3" font-family="Times New Roman,serif" font-size="14.00">utils</text>
</a>
</g>
</g>
<!-- modm_io&#45;&gt;modm_utils -->
<g id="edge3" class="edge"><title>modm_io&#45;&gt;modm_utils</title>
<path fill="none" stroke="black" d="M320.44,-127.142C336.159,-137.572 356.233,-150.956 374,-163 374.763,-163.517 375.534,-164.041 376.311,-164.569"/>
<polygon fill="black" stroke="black" points="374.526,-167.589 384.758,-170.34 378.476,-161.809 374.526,-167.589"/>
</g>
<!-- modm_architecture_accessor&#45;&gt;modm_io -->
<g id="edge4" class="edge"><title>modm_architecture_accessor&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M269.464,-162.997C274.673,-154.487 279.972,-144.972 284.323,-136.328"/>
<polygon fill="black" stroke="black" points="287.609,-137.57 288.777,-127.04 281.297,-134.544 287.609,-137.57"/>
</g>
<!-- modm_architecture_can -->
<g id="node5" class="node"><title>modm_architecture_can</title>
<g id="a_node5"><a xlink:href="../modm-architecture-can" xlink:title="modm:
architecture:
can">
<polygon fill="lightgrey" stroke="black" points="84,-53 0,-53 0,-0 84,-0 84,-53"/>
<text text-anchor="middle" x="42" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="42" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="42" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">can</text>
</a>
</g>
</g>
<!-- modm_architecture_can&#45;&gt;modm_io -->
<g id="edge5" class="edge"><title>modm_architecture_can&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M84.0085,-49.3161C87.0221,-50.6296 90.0394,-51.8732 93,-53 148.002,-73.9344 214.404,-90.1691 255.16,-99.1591"/>
<polygon fill="black" stroke="black" points="254.431,-102.582 264.946,-101.287 255.918,-95.7422 254.431,-102.582"/>
</g>
<!-- modm_architecture_i2c -->
<g id="node6" class="node"><title>modm_architecture_i2c</title>
<g id="a_node6"><a xlink:href="../modm-architecture-i2c" xlink:title="modm:
architecture:
i2c">
<polygon fill="lightgrey" stroke="black" points="186,-53 102,-53 102,-0 186,-0 186,-53"/>
<text text-anchor="middle" x="144" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="144" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="144" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">i2c</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c&#45;&gt;modm_io -->
<g id="edge6" class="edge"><title>modm_architecture_i2c&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M186.299,-50.0689C208.183,-61.7454 234.658,-75.8712 255.761,-87.1308"/>
<polygon fill="black" stroke="black" points="254.274,-90.3044 264.744,-91.9239 257.569,-84.1285 254.274,-90.3044"/>
</g>
<!-- modm_architecture_register -->
<g id="node7" class="node"><title>modm_architecture_register</title>
<g id="a_node7"><a xlink:href="../modm-architecture-register" xlink:title="modm:
architecture:
register">
<polygon fill="lightgrey" stroke="black" points="288,-53 204,-53 204,-0 288,-0 288,-53"/>
<text text-anchor="middle" x="246" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="246" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="246" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">register</text>
</a>
</g>
</g>
<!-- modm_architecture_register&#45;&gt;modm_io -->
<g id="edge7" class="edge"><title>modm_architecture_register&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M261.13,-53.0919C266.228,-61.7163 271.932,-71.3634 277.089,-80.0867"/>
<polygon fill="black" stroke="black" points="274.115,-81.9341 282.217,-88.7611 280.141,-78.3717 274.115,-81.9341"/>
</g>
<!-- modm_container -->
<g id="node8" class="node"><title>modm_container</title>
<g id="a_node8"><a xlink:href="../modm-container" xlink:title="modm:
container">
<polygon fill="lightgrey" stroke="black" points="374,-45.5 306,-45.5 306,-7.5 374,-7.5 374,-45.5"/>
<text text-anchor="middle" x="340" y="-30.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="340" y="-15.3" font-family="Times New Roman,serif" font-size="14.00">container</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_io -->
<g id="edge8" class="edge"><title>modm_container&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M329.335,-45.5398C323.308,-55.7337 315.657,-68.6755 308.932,-80.0514"/>
<polygon fill="black" stroke="black" points="305.862,-78.367 303.786,-88.7564 311.888,-81.9294 305.862,-78.367"/>
</g>
<!-- modm_debug -->
<g id="node9" class="node"><title>modm_debug</title>
<g id="a_node9"><a xlink:href="../modm-debug" xlink:title="modm:
debug">
<polygon fill="lightgrey" stroke="black" points="448,-45.5 392,-45.5 392,-7.5 448,-7.5 448,-45.5"/>
<text text-anchor="middle" x="420" y="-30.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="420" y="-15.3" font-family="Times New Roman,serif" font-size="14.00">debug</text>
</a>
</g>
</g>
<!-- modm_debug&#45;&gt;modm_io -->
<g id="edge9" class="edge"><title>modm_debug&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M394.263,-45.5435C390.51,-48.0851 386.674,-50.6354 383,-53 365.806,-64.0676 346.392,-75.8085 330.009,-85.5057"/>
<polygon fill="black" stroke="black" points="328.091,-82.5735 321.252,-90.6656 331.644,-88.6044 328.091,-82.5735"/>
</g>
<!-- modm_processing_timer -->
<g id="node10" class="node"><title>modm_processing_timer</title>
<g id="a_node10"><a xlink:href="../modm-processing-timer" xlink:title="modm:
processing:
timer">
<polygon fill="lightgrey" stroke="black" points="546,-53 466,-53 466,-0 546,-0 546,-53"/>
<text text-anchor="middle" x="506" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="506" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">processing:</text>
<text text-anchor="middle" x="506" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">timer</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_io -->
<g id="edge10" class="edge"><title>modm_processing_timer&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M465.871,-49.0395C462.895,-50.4345 459.916,-51.7706 457,-53 414.821,-70.7816 364.608,-86.5635 330.954,-96.4045"/>
<polygon fill="black" stroke="black" points="329.703,-93.123 321.069,-99.2635 331.647,-99.8474 329.703,-93.123"/>
</g>
</g>
</svg>

