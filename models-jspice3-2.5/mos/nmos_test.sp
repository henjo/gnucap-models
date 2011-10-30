**NMOS_TEST**
*.options probe
.include cryo_model.lib

mn1 1 11 0 b nm w=30u l=1.25u
mn2 2 12 0 b nm w=30u l=1.25u 
mn3 3 13 0 b nm w=30u l=1.25u 
mn4 4 14 0 b nm w=30u l=1.25u 
mn5 5 15 0 b nm w=30u l=1.25u 
mn6 6 16 0 b nm w=30u l=1.25u 
mn7 7 17 0 b nm w=30u l=1.25u 
mn8 8 18 0 b nm w=30u l=1.25u
mn9 9 19 0 b nm w=30u l=1.25u
mn10 10 20 0 b nm w=30u l=1.25u
 
vds d 0 0
vbs b 0 1.15

v0 d 1 0
v1 d 2 0
v2 d 3 0
v3 d 4 0
v4 d 5 0
v5 d 6 0
v6 d 7 0
v7 d 8 0
v8 d 9 0
v9 d 10 0

vg0 11 0 0
vg1 12 0 .5
vg2 13 0 1
vg3 14 0 1.5
vg4 15 0 2
vg5 16 0 2.5
vg6 17 0 3
vg7 18 0 3.5
vg8 19 0 4
vg9 20 0 4.5

**OUTPUT**
.probe dc v(d) v(b) i(v?)
.dc vds 0 5 1
 
.end












