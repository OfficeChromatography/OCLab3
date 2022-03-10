//suspension + solution dispenser trough

    //Selection: uncomment respective line

//suspension_trough();
solution_trough();
//support();
//cutting_aid();    //silicone mat

x=128;
y=120;
h=10;
platte_x=101;
platte_x_50=104;
platte_y=101;
platte_y_50=51;
$fn=80;

module suspension_trough() {    
    slit = 0.16; //0.16 resulted in a slit width of 0.1 mm
    slit_L=96;
    D=5.4; // OD glass rod 5 mm, length 98 mm
     
    color( "Gray", 1.0 ) {
    difference() {
    union() {
        //bottom
        hull() {
        translate([0, 0, 1.75]) cube([2, 2, 99]);
        translate ([12, 0, 1.75]) cube([2, 2, 99]);
        translate ([5.5, 8, 1.75]) cube([3, 2, 99]);
        //translate ([7, 7, 1.75]) cylinder(r=3, h=99, $fn=100);
            }
        //buildup
        translate([0, -12, 0]) cube([14, 12, 102.5]);
        //hold
        translate([13, -12, 31.25]) cube([10, 2, 40]);
        translate([21, -12, 31.25]) cube([2, 4, 40]);
        }
    //cut-out
    hull() {
    translate([7, 0, 2.25]) cylinder(d=D, h=98);
    translate([2, -12.5, 2.25]) cube([2, 2, 98]);
    translate ([10, -12.5, 2.25]) cube([2, 2, 98]);
        }
    //slit
    translate([7-slit/2, -10, (102.5-slit_L)/2]) cube([slit, 22, slit_L]); 
    //section cut for testing
    //translate([-3, -25, -4]) cube([20, 40, 37]);
    //label
    string = str(slit, " x ", slit_L, " mm, ", D, " mm OD");
    translate ([12, -5, 70]) rotate([0, 90, 0])
    linear_extrude(2) text(string, size = 4, direction = "ltr", spacing = 1 );
    }
        }
 //bridges
    //translate([2.5, -1, 33]) cube([9, 3, 2]);
    //translate([2.5, -1, 66]) cube([9, 3, 2]);
 //glass rod for testing
 /*color( "LightCyan", 1.0 ) {
     translate([7, 0, 2.25]) cylinder(d=5, h=98);
 }*/
 }

module solution_trough() {
color( "Gray", 1.0 ) {
    D1=5.2; // OD glass rod 5 mm, length 99 mm
    D2=5.3; //cut-out
    
    difference() {
    union() {
        //bottom
        hull() {
        translate([0, 0, 1.75]) cube([2, 2, 99]);
        translate ([12, 0, 1.75]) cube([2, 2, 99]);
        translate ([7, 7, 1.75]) cylinder(r=3.5, h=99, $fn=100);
            }
        //buildup
        translate([0, -8, 0]) cube([14, 8, 102.5]);
        //hold
        translate([13, -8, 31.25]) cube([10, 2, 40]);
        translate([21, -8, 31.25]) cube([2, 4, 40]);
        }
    //cut-out
    hull() {
    translate([7, 2.3, 4.25]) cylinder(d=D2, h=94); //change the translate y value to modify the slit above the glass rod (minimum=2.3), the diameter D2 also could be increased/decreased (D2=5.1 results in a very small slit above the glass rod). With D1=5.2, the trough is well suited to apply bioluminescent bacteria.
    translate([2, -8.5, 3.25]) cube([2, 2, 96]);
    translate ([10, -8.5, 3.25]) cube([2, 2, 96]);
        }
    translate([7, 7.4, -1]) cylinder(d=D1, h=104);
    //flat cut
    translate([-5, 9.3, -2]) cube([20, 2, 110]);
    //section cut for testing
    //translate([-3, -25, -4]) cube([30, 40, 70]);
    //label
    string = str("D1= ", D1, ", D2= ", D2, "mm");
    translate ([12, -5, 60]) rotate([0, 90, 0])
    linear_extrude(2) text(string, size = 4, direction = "ltr", spacing = 1 );
 }
 }
//bridges
translate([2.5, -1, 33]) cube([9, 3, 2]);
translate([2.5, -1, 66]) cube([9, 3, 2]);
//glass rod for testing
 /*color( "LightCyan", 1.0 ) {
     translate([7, 7.4, 1.3]) cylinder(d=5, h=100);
 }*/
 }

module hanger() {
    difference() {
        difference() {
        //cube([6, 15, 62.5]);
        #translate([0, 35, 0]) cube([6, 12, 62.5]);
        hull() {
        translate([-1, 39.2, 7.5]) rotate([0, 90, 0]) cylinder(r=4.1, h=8, $fn=60);
        translate([-1, 39.2, -7.5]) rotate([0, 90, 0]) cylinder(r=4.1, h=8, $fn=60);
        }
        hull() {
        translate([-1, 39.2, 7.5+45]) rotate([0, 90, 0]) cylinder(r=4.1, h=8, $fn=60);
       translate([-1, 39.2, 25]) rotate([0, 90, 0]) cylinder(r=4.1, h=8, $fn=60);
        }
    translate([0, 25, 17.5]) cube([6, 25, 27.5]);
    translate([0, 25, 0]) cube([6, 25, 7]);
    }
        cube([6, 35, 62.5]); //Abschneiden
    }  
}

module support() {
    difference() {
        union() {
        cube([110, 18.5, 72]);
        translate([18, 18-34.6, 9.5]) hanger();
        translate([86, 18-34.6, 9.5]) hanger();
        }
        translate([3.25, 2, 0]) cube([103.5, 14.5, 75]);
        translate([5, -1, 0]) cube([100, 5, 75]);
        translate([3.25, -1, 35]) cube([103.5, 5, 75]);
    }
}

module cutting_aid() {   //silicone mat
k=0.8; //knife thickness
HF=10; //height
LF = 114; //length
DF = 3; //thickness
difference() {
cube([150, 45, 10]);
translate([18, 0, 10-DF]) cube([LF, 28, 10]); 
translate([5, 28-HF-k, 0]) cube([140, k, 10]);  
string = str(LF, " x ", HF, " x ", DF, " mm");
    translate ([12, 35, 7])
    linear_extrude(4) text(string, size = 6, direction = "ltr", spacing = 1 );    
}
}