//valve holder OCLab3
$fn=80;

    //select, uncomment the line
//x_holder();
translate([5.5, 7, 5.3]) valve_holder();

module x_holder() {
difference() {
    translate([-1, 0, 0]) cube([30, 80, 5.5]);
    translate([-1, 21, 2]) cube([30, 80, 10]);  
    //translate([4, 22, -2]) cylinder(r=1.5, h=10);  
    //translate([24, 22, -2]) cylinder(r=1.5, h=10);  
    translate([2, 12, -2]) cube([3.5, 60, 10]);  
    translate([22, 12, -2]) cube([3.5, 60, 10]);  
    translate([7, 7-4, -2]) cylinder(d=2, h=10);
    translate([7, 7, -2]) cylinder(d=2, h=10);
    translate([7, 7+4, -2]) cylinder(d=2, h=10);
    translate([7, 7+4+4, -2]) cylinder(d=2, h=10);
    translate([7, 7+4+4+4, -2]) cylinder(d=2, h=10);
    
    translate([7+13, 7-4, -2]) cylinder(d=2, h=10);
    translate([7+13, 7, -2]) cylinder(d=2, h=10);
    translate([7+13, 7+4, -2]) cylinder(d=2, h=10);
    translate([7+13, 7+4+4, -2]) cylinder(d=2, h=10);
    translate([7+13, 7+4+4+4, -2]) cylinder(d=2, h=10);
    
    translate([14, 21, 5.5]) rotate([90, 0, 0]) cylinder(d=8, h=23);
}
//translate([-5, -5, 0]) cube([30, 5, 1.5]);
}

module x_holder_2() {  //verstellbar
difference() {
    translate([-1, 0, 0]) cube([30, 80, 5.5]);
    translate([-1, 21, 2]) cube([30, 80, 10]);  
    //translate([4, 22, -2]) cylinder(r=1.5, h=10);  
    //translate([24, 22, -2]) cylinder(r=1.5, h=10);  
    translate([2, 22, -2]) cube([3.1, 50, 6]);  
    translate([22, 22, -2]) cube([3.1, 50, 6]);  
    translate([7, 7-4, -2]) cylinder(d=2, h=10);
    translate([7, 7, -2]) cylinder(d=2, h=10);
    translate([7, 7+4, -2]) cylinder(d=2, h=10);
    translate([7, 7+4+4, -2]) cylinder(d=2, h=10);
    translate([7, 7+4+4+4, -2]) cylinder(d=2, h=10);
    
    translate([7+13, 7-4, -2]) cylinder(d=2, h=10);
    translate([7+13, 7, -2]) cylinder(d=2, h=10);
    translate([7+13, 7+4, -2]) cylinder(d=2, h=10);
    translate([7+13, 7+4+4, -2]) cylinder(d=2, h=10);
    translate([7+13, 7+4+4+4, -2]) cylinder(d=2, h=10);
    
    translate([14, 21, 5.5]) rotate([90, 0, 0]) cylinder(d=8, h=23);
}
//translate([-5, -5, 0]) cube([30, 5, 1.5]);
/*difference() {
translate([10, 60, 2]) cube([8, 12, 8]);
translate([14, 55, 6]) rotate([-90, 0, 0]) cylinder(r=1.3, h=25); */
//Mit Mutter
difference() {
    translate([10, 60, 2]) cube([8, 12, 8]);
    translate([14, 55, 6]) rotate([-90, 0, 0]) cylinder(r=1.6, h=25);
    #translate([8, 65, 2]) cube([9, 2.3, 9]);
}
}

//adjustor mit 3D-Klebeband auf x-moving befestigen
module adjustor() {
    $fn=80;
    difference() {
        union() {
    cube([15, 10, 22]);
    translate([0, -10, 15]) cube([15, 10, 7]);
        }
    translate([7.5, -6, 0]) cylinder(r=1.5, h=25);
    translate([7.5, -7.5, 0]) cube([10, 3, 18]);
    translate([7.5, -6, 17.1]) cylinder(r=3, h=2.7);
    translate([7.5, -6-3, 17.1]) cube([20, 6, 2.7]);
    //ehemalige Öffnungen für Schrauben
    /*translate([4, 5, 3]) cylinder(r=2.6, h=22);
    translate([11, 5, 3]) cylinder(r=2.6, h=22);
    translate([4, 5, -1]) cylinder(r=1.3, h=22);
    translate([11, 5, -1]) cylinder(r=1.3, h=22); */
    }
}

module valve_holder() {
    difference() {
        union() {
    cube([17, 8, 2]);
    translate([8.5, -5, 0]) rotate([-90, 0, 0]) cylinder(d=7.80, h=19);
    translate([8.5, 3, 2.6]) cylinder(r=3, h=3);
        }
    translate([2, 4, -1]) cylinder(d=2, h=5);
    translate([2+13, 4, -1]) cylinder(d=2, h=5);
    //translate([2, 2+4, -1]) cylinder(d=2, h=5);
    //translate([2+13, 2+4, -1]) cylinder(d=2, h=5);
    
    translate([8.5, -8, 0]) rotate([-90, 0, 0]) cylinder(d=5.9, h=25);
    //translate([9, -5, 0]) rotate([-90, 0, 0]) cylinder(d=5, h=20);
    translate([8.5, 3, 2]) cylinder(r=1.4, h=5);
    }
}

module valve_holder_2() {
    
    import("C:/Users/wschwack/GitHub/OCLab3/OCLab3/3D Parts/STLFiles/Axis/X/coatSupport.stl");
}
    