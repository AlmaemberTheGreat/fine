# `fine`, a lightweight benchmarking tool

I was using [`hyperfine`][1] for a while, but then I noticed one thing: it was pretty big (more than 2000 LoC), written in Rust, and non-Unixy.
It also had a bunch of features I didn't need.

So I decided to rewrite some of it in C. The result is a (I think) Unixy tool that can do much of the same in much less code. Enjoy!

(P.S. I have nothing against `hyperfine`, I just wanted to make my own.)

[1]: https://github.com/sharkdp/hyperfine
