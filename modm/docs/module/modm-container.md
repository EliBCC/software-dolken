!!! warning "These module docs are in beta and may be incomplete."

# modm:container: Containers

A container is a holder object that stores a collection other objects (its
elements). They are implemented as class templates, which allows a great
flexibility in the types supported as elements.

Many containers have several member functions in common, and share
functionalities. The decision of which type of container to use for a specific
need does not generally depend only on the functionality offered by the
container, but also on the efficiency of some of its members (complexity).
This is especially true for sequence containers, which offer different
trade-offs in complexity between inserting/removing elements and accessing
them.

`modm::Stack` and `modm::Queue` are implemented as container adapters. Container
adapters are not full container classes, but classes that provide a specific
interface relying on an object of one of the container classes (such as
`modm::BoundedDeque` or `modm::LinkedList`) to handle the elements.
The underlying container is encapsulated in such a way that its elements are
accessed by the members of the container class independently of the underlying
container class used.

Sequence containers:

- `modm::DynamicArray`
- `modm::LinkedList`
- `modm::DoublyLinkedList`
- `modm::BoundedDeque`

Container adapters:

- `modm::Queue`
- `modm::Stack`
- `modm::BoundedStack`
- `modm::BoundedQueue`

Other:

- `modm::SmartPointer`
- `modm::Pair`

Two special containers hiding in the `modm:architecture:atomic` module:

- `modm::atomic::Queue`
- `modm::atomic::Container`

The first is a simple, interrupt-safe queue (but only for the AVRs).
Whenever you need to exchange data between a interrupt routine and the normal
program consider using this queue.

The atomic container wraps objects and provides atomic access to
them. This comes in handy when simple objects are accessed by an interrupt
and the main program. The container provides secure access without much work
in this case.

## Generic Interface

All implementation share a common set of function. Not every container implement
every one of it, only a subset which is sufficient for the container.

- append
- prepend
- (insert)
- removeFront
- removeBack
- (remove)
- getFront
- getBack

- push (only Stack and Queue)
- pop (only Stack and Queue)
- get (only Stack and Queue)

- at
- operator[]

- clear

- isEmpty
- isFull
- getSize
- getMaxSize
- getCapacity






## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.38.0 (20140413.2041)
 -->
<!-- Title: modm:container Pages: 1 -->
<svg width="238pt" height="209pt"
 viewBox="0.00 0.00 237.50 209.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 205)">
<title>modm:container</title>
<polygon fill="white" stroke="none" points="-4,4 -4,-205 233.5,-205 233.5,4 -4,4"/>
<!-- modm_container -->
<g id="node1" class="node"><title>modm_container</title>
<polygon fill="lightgrey" stroke="black" stroke-width="2" points="161.5,-127 93.5,-127 93.5,-89 161.5,-89 161.5,-127"/>
<text text-anchor="middle" x="127.5" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="127.5" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">container</text>
</g>
<!-- modm_architecture -->
<g id="node2" class="node"><title>modm_architecture</title>
<g id="a_node2"><a xlink:href="../modm-architecture" xlink:title="modm:
architecture">
<polygon fill="lightgrey" stroke="black" points="81,-201 0,-201 0,-163 81,-163 81,-201"/>
<text text-anchor="middle" x="40.5" y="-185.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="40.5" y="-170.8" font-family="Times New Roman,serif" font-size="14.00">architecture</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_architecture -->
<g id="edge1" class="edge"><title>modm_container&#45;&gt;modm_architecture</title>
<path fill="none" stroke="black" d="M105.546,-127.169C94.8305,-136.037 81.7811,-146.836 70.191,-156.428"/>
<polygon fill="black" stroke="black" points="67.9192,-153.765 62.4467,-162.837 72.3822,-159.158 67.9192,-153.765"/>
</g>
<!-- modm_io -->
<g id="node3" class="node"><title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:
io">
<polygon fill="lightgrey" stroke="black" points="155.5,-201 99.5,-201 99.5,-163 155.5,-163 155.5,-201"/>
<text text-anchor="middle" x="127.5" y="-185.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="127.5" y="-170.8" font-family="Times New Roman,serif" font-size="14.00">io</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_io -->
<g id="edge2" class="edge"><title>modm_container&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M127.5,-127.169C127.5,-134.869 127.5,-144.026 127.5,-152.583"/>
<polygon fill="black" stroke="black" points="124,-152.587 127.5,-162.587 131,-152.587 124,-152.587"/>
</g>
<!-- modm_utils -->
<g id="node4" class="node"><title>modm_utils</title>
<g id="a_node4"><a xlink:href="../modm-utils" xlink:title="modm:
utils">
<polygon fill="lightgrey" stroke="black" points="229.5,-201 173.5,-201 173.5,-163 229.5,-163 229.5,-201"/>
<text text-anchor="middle" x="201.5" y="-185.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="201.5" y="-170.8" font-family="Times New Roman,serif" font-size="14.00">utils</text>
</a>
</g>
</g>
<!-- modm_container&#45;&gt;modm_utils -->
<g id="edge3" class="edge"><title>modm_container&#45;&gt;modm_utils</title>
<path fill="none" stroke="black" d="M146.173,-127.169C155.025,-135.781 165.749,-146.215 175.389,-155.595"/>
<polygon fill="black" stroke="black" points="173.225,-158.372 182.833,-162.837 178.106,-153.355 173.225,-158.372"/>
</g>
<!-- modm_platform_i2c -->
<g id="node5" class="node"><title>modm_platform_i2c</title>
<g id="a_node5"><a xlink:href="../modm-platform-i2c" xlink:title="modm:
platform:
i2c">
<polygon fill="lightgrey" stroke="black" points="161,-53 94,-53 94,-0 161,-0 161,-53"/>
<text text-anchor="middle" x="127.5" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="127.5" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">platform:</text>
<text text-anchor="middle" x="127.5" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">i2c</text>
</a>
</g>
</g>
<!-- modm_platform_i2c&#45;&gt;modm_container -->
<g id="edge4" class="edge"><title>modm_platform_i2c&#45;&gt;modm_container</title>
<path fill="none" stroke="black" d="M127.5,-53.0919C127.5,-61.2623 127.5,-70.3508 127.5,-78.7019"/>
<polygon fill="black" stroke="black" points="124,-78.7611 127.5,-88.7611 131,-78.7611 124,-78.7611"/>
</g>
</g>
</svg>
