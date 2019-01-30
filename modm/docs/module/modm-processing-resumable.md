!!! warning "These module docs are in beta and may be incomplete."

# modm:processing:resumable: Resumable Functions

An implementation of lightweight resumable functions which allow for nested calling.

This base class and its macros allows you to implement and use several
resumable functions in one class.
This allows you to modularize your code by placing it into its own resumable functions
instead of the placing everything into one big method.
It also allows you to call and run resumable functions within your resumables,
so you can reuse their functionality.

Note that you should call resumable functions within a protothreads, with the
`PT_CALL(group.resumable())` macro, which will return the result of the
resumable function. To call a resumable function inside another resumable
function, use the `RF_CALL(group.resumable())`.

You may use the `RF_CALL_BLOCKING(group.resumable())` macro to execute
a resumable function outside of a protothread, however, this which will
force the CPU to busy-wait until the resumable function ended.

!!! warning "Resumable functions are not thread-safe!"
    Use an external mutex to arbitrate access if two Protothreads access the
    same resumable function.

You can either run your group of resumable functions independently from each
other, or only run one function at a time, but it may nest calls to its own
group's functions.
So you need to inherit your group from `modm::Resumable<#Functions>`, and from
`modm::NestedResumable<#Functions>` respectively.


## Independent operation

You must begin each resumable function using `RF_BEGIN(index)` where `index` is
the unique index of your resumable function starting at zero.
You may exit and return a value by using `RF_RETURN(value)` or
return the result of another resumable function using `RF_RETURN_CALL(resumable())`.
This return value is wrapped in a `modm::ResumableResult<Type>` struct
and transparently returned by the `RF_CALL` macro so it can be used
to influence your program flow.
If the resumable function reaches `RF_END()` it will exit automatically,
with the result of `0` cast to the return type.
Should you wish to return a value at the end, you may use
`RF_END_RETURN(value)`.
You may also return the result of another resumable function using
`RF_END_RETURN_CALL(resumable())`.

Be aware that this class keeps a separate state for each of your resumable functions.
This allows each resumable function to be run at the same time.
This might require the use of an internal semaphore or mutex if such
dependencies exist in your use case.
Take a look at the `NestedResumable` class for mutually exclusive resumable functions,
which also require a little less memory.


## Nested operation

You are responsible to choosing the right nesting depth!
This class will guard itself against calling another resumable function at too
deep a nesting level and fail the `resumable.begin.nesting` assertion!
It is then up to you to recognize this in your program design
and increase the nesting depth or rethink your code.

The resumable functions of this class are mutually exclusive, so only one
resumable function of the same object can run at the same time. Even if you
call another resumable function, it will simply return `modm::rf::WrongState`.
Using the `RF_CALL(resumable())` macro, you can wait for these
resumable functions to become available and then run them, so you usually do
not need to worry about those cases.

You must begin each resumable function using `RF_BEGIN()`.
You may exit and return a value by using `RF_RETURN(value)` or
return the result of another resumable function using `RF_RETURN_CALL(resumable())`.
This return value is wrapped in a `modm::ResumableResult<Type>` struct
and transparently returned by the `RF_CALL` macro so it can be used
to influence your program flow.
If the resumable function reaches `RF_END()` it will exit automatically,
with the result of `0` cast to the return type.
Should you wish to return a value at the end, you may use
`RF_END_RETURN(value)`.
You may also return the result of another resumable function using
`RF_END_RETURN_CALL(resumable())`.


## Example

Here is a (slightly over-engineered) example:

```cpp
#include <modm/platform/platform.hpp>
#include <modm/processing/processing.hpp>

using Led = GpioOutputB0;

class BlinkingLight : public modm::pt::Protothread, private modm::NestedResumable<2>
{
public:
    bool
    run()
    {
        PT_BEGIN();

        // set everything up
        Led::setOutput();
        Led::set();

        while (true)
        {
            Led::set();
            PT_CALL(waitForTimer());

            Led::reset();
            PT_CALL(setTimer(200));

            PT_WAIT_UNTIL(timeout.isExpired());
        }

        PT_END();
    }

    modm::ResumableResult<bool>
    waitForTimer()
    {
        RF_BEGIN();

        // nested calling is allowed
        if (RF_CALL(setTimer(100)))
        {
            RF_WAIT_UNTIL(timeout.isExpired());
            RF_RETURN(true);
        }

        RF_END_RETURN(false);
    }

    modm::ResumableResult<bool>
    setTimer(uint16_t new_timeout)
    {
        RF_BEGIN();

        timeout.restart(new_timeout);

        if(timeout.isArmed()) {
            RF_RETURN(true);
        }

        // clean up code goes here

        RF_END_RETURN(false);
    }

private:
    modm::ShortTimeout timeout;
};

BlinkingLight light;

while (1) {
    light.run();
}
```

For other examples take a look in the `examples` folder in the modm
root folder. The given example is in `modm/examples/generic/resumable`.


## Options

#### check_nesting_depth

Check nesting call depth: `True` ∈ `{ [4mTrue[24m, False[0m }`

Nested resumable functions protect against memory corruption by checking if the
nesting level is within the allocated nesting level depth, on first entry to
the function. If the allocated nesting level is exceeded, the assertion
`resumable.begin.nesting` fails.

You may disable this behavior by disabling this check, then instead of the
assertion, the function on entry returns the `modm::rf::NestingError` state value.
`PT_CALL()` and `RF_CALL()` macros will respond to this error by stopping
function polling and just continuing program execution.

!!! info "Performance Penalty"
    This check is performed during the call to `RF_BEGIN(N)`, so exactly once
    on function entry and not during every polling call, so the performance
    penalty is relatively small.






## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.38.0 (20140413.2041)
 -->
<!-- Title: modm:processing:resumable Pages: 1 -->
<svg width="194pt" height="239pt"
 viewBox="0.00 0.00 194.00 239.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 235)">
<title>modm:processing:resumable</title>
<polygon fill="white" stroke="none" points="-4,4 -4,-235 190,-235 190,4 -4,4"/>
<!-- modm_processing_resumable -->
<g id="node1" class="node"><title>modm_processing_resumable</title>
<polygon fill="lightgrey" stroke="black" stroke-width="2" points="133,-142 53,-142 53,-89 133,-89 133,-142"/>
<text text-anchor="middle" x="93" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="93" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">processing:</text>
<text text-anchor="middle" x="93" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">resumable</text>
</g>
<!-- modm_architecture_assert -->
<g id="node2" class="node"><title>modm_architecture_assert</title>
<g id="a_node2"><a xlink:href="../modm-architecture-assert" xlink:title="modm:
architecture:
assert">
<polygon fill="lightgrey" stroke="black" points="86,-231 2,-231 2,-178 86,-178 86,-231"/>
<text text-anchor="middle" x="44" y="-215.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="44" y="-200.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="44" y="-185.8" font-family="Times New Roman,serif" font-size="14.00">assert</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_architecture_assert -->
<g id="edge1" class="edge"><title>modm_processing_resumable&#45;&gt;modm_architecture_assert</title>
<path fill="none" stroke="black" d="M78.5634,-142.132C73.8208,-150.553 68.4692,-160.055 63.4189,-169.022"/>
<polygon fill="black" stroke="black" points="60.3606,-167.32 58.5027,-177.75 66.4597,-170.755 60.3606,-167.32"/>
</g>
<!-- modm_processing -->
<g id="node3" class="node"><title>modm_processing</title>
<g id="a_node3"><a xlink:href="../modm-processing" xlink:title="modm:
processing">
<polygon fill="lightgrey" stroke="black" points="181.5,-223.5 104.5,-223.5 104.5,-185.5 181.5,-185.5 181.5,-223.5"/>
<text text-anchor="middle" x="143" y="-208.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="143" y="-193.3" font-family="Times New Roman,serif" font-size="14.00">processing</text>
</a>
</g>
</g>
<!-- modm_processing_resumable&#45;&gt;modm_processing -->
<g id="edge2" class="edge"><title>modm_processing_resumable&#45;&gt;modm_processing</title>
<path fill="none" stroke="black" d="M107.731,-142.132C113.988,-153.018 121.282,-165.711 127.609,-176.719"/>
<polygon fill="black" stroke="black" points="124.634,-178.567 132.651,-185.493 130.703,-175.079 124.634,-178.567"/>
</g>
<!-- modm_architecture_i2c_device -->
<g id="node4" class="node"><title>modm_architecture_i2c_device</title>
<g id="a_node4"><a xlink:href="../modm-architecture-i2c-device" xlink:title="modm:
architecture:
i2c.device">
<polygon fill="lightgrey" stroke="black" points="84,-53 0,-53 0,-0 84,-0 84,-53"/>
<text text-anchor="middle" x="42" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="42" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="42" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">i2c.device</text>
</a>
</g>
</g>
<!-- modm_architecture_i2c_device&#45;&gt;modm_processing_resumable -->
<g id="edge3" class="edge"><title>modm_architecture_i2c_device&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="black" d="M57.0258,-53.1323C61.9621,-61.5529 67.5321,-71.0547 72.7885,-80.0216"/>
<polygon fill="black" stroke="black" points="69.8286,-81.8933 77.9053,-88.7503 75.8675,-78.3532 69.8286,-81.8933"/>
</g>
<!-- modm_architecture_spi -->
<g id="node5" class="node"><title>modm_architecture_spi</title>
<g id="a_node5"><a xlink:href="../modm-architecture-spi" xlink:title="modm:
architecture:
spi">
<polygon fill="lightgrey" stroke="black" points="186,-53 102,-53 102,-0 186,-0 186,-53"/>
<text text-anchor="middle" x="144" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="144" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="144" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">spi</text>
</a>
</g>
</g>
<!-- modm_architecture_spi&#45;&gt;modm_processing_resumable -->
<g id="edge4" class="edge"><title>modm_architecture_spi&#45;&gt;modm_processing_resumable</title>
<path fill="none" stroke="black" d="M128.974,-53.1323C124.038,-61.5529 118.468,-71.0547 113.211,-80.0216"/>
<polygon fill="black" stroke="black" points="110.132,-78.3532 108.095,-88.7503 116.171,-81.8933 110.132,-78.3532"/>
</g>
</g>
</svg>

