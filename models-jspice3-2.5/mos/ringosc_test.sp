**RINGOSC_TEST**
**A 13-stages ring oscillator, each stage with 3x load**
*.options probe
.include cryo_model.lib

.subckt inv i o z
mn1 o i 0 0 nm w=5u l=1.1u 
mp1 o i z z pm w=10u l=1.5u
.ends inv

.subckt inv2 i o z
mn1 o i 0 0 nm w=10u l=1.1u 
mp1 o i z z pm w=20u l=1.5u
.ends inv2

x1 1 2 vdd inv
x1b 1 2b vdd inv2
x2 2 3 vdd inv
x2b 2 3b vdd inv2
x3 3 4 vdd inv
x3b 3 4b vdd inv2
x4 4 5 vdd inv
x4b 4 5b vdd inv2
x5 5 6 vdd inv
x5b 5 6b vdd inv2
x6 6 7 vdd inv
x6b 6 7b vdd inv2
x7 7 8 vdd inv
x7b 7 8b vdd inv2
x8 8 9 vdd inv
x8b 8 9b vdd inv2
x9 9 10 vdd inv
x9b 9 10b vdd inv2
x10 10 11 vdd inv
x10b 10 11b vdd inv2
x11 11 12 vdd inv
x11b 11 12b vdd inv2
x12 12 13 vdd inv
x12b 12 13b vdd inv2
x13 13 1 vdd inv
x13b 13 1b vdd inv2

.ic v(1)=0 v(2)=1 v(3)=0 v(4)=1 v(5)=0
+ v(6)=1 v(7)=0 v(8)=1 v(9)=0 v(10)=1
+ v(11)=0 v(12)=1

vdd vdd 0 1v

**OUTPUT**
.probe tran v(nodes)
.tran .05ns 30ns uic trace iter

.end



