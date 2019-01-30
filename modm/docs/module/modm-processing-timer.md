!!! warning "These module docs are in beta and may be incomplete."

# modm:processing:timer: Software Timers

An implementation of lightweight software timeouts and periodic timers.

- `modm::Timestamp` containing a time in millisecond resolution up to 49 days.
- `modm::Timeout` for timeouts up to 24 days.
- `modm::PeriodicTimer` for periodic timeouts up to 24 days periods.

There are also 16-bit versions of these, in case you need to preserve memory:

- `modm::ShortTimestamp` containing a time in millisecond resolution up to 65 seconds.
- `modm::ShortTimeout` for timeouts up to 32 seconds.
- `modm::ShortPeriodicTimer` for periodic timeouts up to 32 second periods.

These classes default to using `modm::Clock`, which must be implemented on your
system to return a time with millisecond resolution.

You may also create your own timers with custom time bases and resolutions
using these classes:

- `modm::GenericTimestamp`.
- `modm::GenericTimeout`.
- `modm::GenericPeriodicTimer`.

!!! warning
	Never use these classes when a precise timebase is needed!

## Timeouts

The `modm::GenericTimeout` classes allow for a signal to be generated after a
period of time, which can also be used to execute code once after timeout
expiration.

Its behavior can be described by the following annotated waveform:

- C: Default Constructor
- S: (Re-)Start timeout
- E: Timeout Expired
- H: Code handler (`execute()` returned `true`)
- P: Stop timeout

```
Event:    C      S      E     H       P            S          E H
                         _____________                         ______________
Expired:  ______________/             \_______________________/              ...
                  ______                            __________
Armed:    _______/      \__________________________/          \______________...
          _______                      ____________
Stopped:         \____________________/            \_________________________...

                              _                                 _
Handle:   ___________________/ \_______________________________/ \___________...

Remaining:   0  |   +   |      -      |     0      |     +     |   -
```

The default constructor initializes the timeout in the `Stopped` state,
in which `isExpired()` and `execute()` never return `true`.
If you need a timeout to expire immidiately after construction, you need
to explicitly initialize the constructor with time `0`, which has the
same behavior as `restart(0)`.

If you want to execute code once after the timeout expired, poll the
`execute()` method, which returns `true` exactly once after expiration.

```cpp
if (timeout.execute())
{
    // called once after timeout
    Led::toggle();
}
```

Be aware, however, that since this method is polled, it cannot execute
exactly at the time of expiration, but some time after expiration, as
indicated in the above waveform graph.

The `remaining()` time until expiration is signed positive before, and
negative after expiration. This means `Clock::now() + Timeout::remaining()`
will yield the timestamp of the expiration.
If the timeout is stopped, `remaining()` returns zero.

## Periodic Timers

The `modm::GenericPeriodicTimer` class allows for periodic code execution.

Its behavior can be described by the following annotated waveform:

- C: Constructor
- S: (Re-)Start timer
- I: Period interval
- H: Code handler (`execute()` returned `true`)
- P: Stop timer

```
Event:    C         IH        I         I  H      I  S    IH   I    IH  P
                     _         _____________       __      _    ______
Expired:  __________/ \_______/             \_____/  \____/ \__/      \____...
          __________   _______               _____    ____   __        _
Armed:              \_/       \_____________/     \__/    \_/  \______/ \__...
                                                                         __
Stopped:  ______________________________________________________________/  ...

                     _                     _               _         _
Handle:   __________/ \___________________/ \_____________/ \_______/ \____...

Remaining:     +    |0|   +   |     -     |0|  +  | -|  + |0| +| -  |0|+| 0
```
 *
 * If you want to execute code once per period interval, poll the
 * `execute()` method, which returns `true` exactly once after expiration.
 *
 * @code
 * if (timer.execute())
 * {
 *     // periodically called once
 *     Led::toggle();
 * }
 * @endcode
 *
 * Be aware, however, that since this method is polled, it cannot execute
 * exactly at the time of expiration, but some time after expiration, as
 * indicated in the above waveform graph.
 * If one or several periods are missed when polling `execute()`, these
 * code executions are discarded and will not be caught up.
 * Instead, `execute()` returns `true` once and then reschedules itself
 * for the next period, without any period skewing.
 *
 * @warning	Never use this class when a precise timebase is needed!
 *
 * Notice, that the `PeriodicTimerState::Expired` is reset to
 * `PeriodicTimerState::Armed` only after `execute()` has returned `true`.
 * This is different to the behavior of GenericTimeout, where calls to
 * `GenericTimeout::execute()` have no impact on class state.
 *
 * The `remaining()` time until period expiration is signed positive before,
 * and negative after period expiration until `execute()` is called.
 * If the timer is stopped, `remaining()` returns zero.






## Dependencies

<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<!-- Generated by graphviz version 2.38.0 (20140413.2041)
 -->
<!-- Title: modm:processing:timer Pages: 1 -->
<svg width="336pt" height="150pt"
 viewBox="0.00 0.00 336.00 150.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
<g id="graph0" class="graph" transform="scale(1 1) rotate(0) translate(4 146)">
<title>modm:processing:timer</title>
<polygon fill="white" stroke="none" points="-4,4 -4,-146 332,-146 332,4 -4,4"/>
<!-- modm_processing_timer -->
<g id="node1" class="node"><title>modm_processing_timer</title>
<polygon fill="lightgrey" stroke="black" stroke-width="2" points="170,-53 90,-53 90,-0 170,-0 170,-53"/>
<text text-anchor="middle" x="130" y="-37.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="130" y="-22.8" font-family="Times New Roman,serif" font-size="14.00">processing:</text>
<text text-anchor="middle" x="130" y="-7.8" font-family="Times New Roman,serif" font-size="14.00">timer</text>
</g>
<!-- modm_architecture_clock -->
<g id="node2" class="node"><title>modm_architecture_clock</title>
<g id="a_node2"><a xlink:href="../modm-architecture-clock" xlink:title="modm:
architecture:
clock">
<polygon fill="lightgrey" stroke="black" points="84,-142 0,-142 0,-89 84,-89 84,-142"/>
<text text-anchor="middle" x="42" y="-126.8" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="42" y="-111.8" font-family="Times New Roman,serif" font-size="14.00">architecture:</text>
<text text-anchor="middle" x="42" y="-96.8" font-family="Times New Roman,serif" font-size="14.00">clock</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_architecture_clock -->
<g id="edge1" class="edge"><title>modm_processing_timer&#45;&gt;modm_architecture_clock</title>
<path fill="none" stroke="black" d="M97.857,-53.1323C88.3362,-61.9915 78.045,-72.0474 68.8873,-81.4181"/>
<polygon fill="black" stroke="black" points="66.2406,-79.1206 61.8223,-88.7503 71.2813,-83.9777 66.2406,-79.1206"/>
</g>
<!-- modm_io -->
<g id="node3" class="node"><title>modm_io</title>
<g id="a_node3"><a xlink:href="../modm-io" xlink:title="modm:
io">
<polygon fill="lightgrey" stroke="black" points="158,-134.5 102,-134.5 102,-96.5 158,-96.5 158,-134.5"/>
<text text-anchor="middle" x="130" y="-119.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="130" y="-104.3" font-family="Times New Roman,serif" font-size="14.00">io</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_io -->
<g id="edge2" class="edge"><title>modm_processing_timer&#45;&gt;modm_io</title>
<path fill="none" stroke="black" d="M130,-53.1323C130,-63.5829 130,-75.6988 130,-86.3906"/>
<polygon fill="black" stroke="black" points="126.5,-86.4933 130,-96.4933 133.5,-86.4934 126.5,-86.4933"/>
</g>
<!-- modm_processing -->
<g id="node4" class="node"><title>modm_processing</title>
<g id="a_node4"><a xlink:href="../modm-processing" xlink:title="modm:
processing">
<polygon fill="lightgrey" stroke="black" points="253.5,-134.5 176.5,-134.5 176.5,-96.5 253.5,-96.5 253.5,-134.5"/>
<text text-anchor="middle" x="215" y="-119.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="215" y="-104.3" font-family="Times New Roman,serif" font-size="14.00">processing</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_processing -->
<g id="edge3" class="edge"><title>modm_processing_timer&#45;&gt;modm_processing</title>
<path fill="none" stroke="black" d="M155.043,-53.1323C166.104,-64.4538 179.075,-77.7297 190.118,-89.0321"/>
<polygon fill="black" stroke="black" points="187.915,-91.7864 197.407,-96.4933 192.922,-86.8946 187.915,-91.7864"/>
</g>
<!-- modm_utils -->
<g id="node5" class="node"><title>modm_utils</title>
<g id="a_node5"><a xlink:href="../modm-utils" xlink:title="modm:
utils">
<polygon fill="lightgrey" stroke="black" points="328,-134.5 272,-134.5 272,-96.5 328,-96.5 328,-134.5"/>
<text text-anchor="middle" x="300" y="-119.3" font-family="Times New Roman,serif" font-size="14.00">modm:</text>
<text text-anchor="middle" x="300" y="-104.3" font-family="Times New Roman,serif" font-size="14.00">utils</text>
</a>
</g>
</g>
<!-- modm_processing_timer&#45;&gt;modm_utils -->
<g id="edge4" class="edge"><title>modm_processing_timer&#45;&gt;modm_utils</title>
<path fill="none" stroke="black" d="M170.167,-43.9465C197.026,-55.4554 232.836,-71.7979 263,-89 264.105,-89.6304 265.22,-90.2841 266.339,-90.9554"/>
<polygon fill="black" stroke="black" points="264.567,-93.9759 274.891,-96.3564 268.305,-88.0574 264.567,-93.9759"/>
</g>
<!-- modm_architecture_clock&#45;&gt;modm_processing_timer -->
<g id="edge5" class="edge"><title>modm_architecture_clock&#45;&gt;modm_processing_timer</title>
<path fill="none" stroke="black" d="M74.2692,-88.7503C83.7968,-79.8805 94.0863,-69.8224 103.234,-60.4578"/>
<polygon fill="black" stroke="black" points="105.873,-62.763 110.289,-53.1323 100.831,-57.9072 105.873,-62.763"/>
</g>
</g>
</svg>

