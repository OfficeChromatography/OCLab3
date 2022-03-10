//valve holder OCLab3
$fn=80;

    //select, uncomment the line
x_holder();
//translate([5.5, 7, 5.3]) valve_holder();

module x_holder() {
difference() {
    translate([0, 0, 0]) cube([28, 80, 5.5]);
    translate([0, 21, 2]) cube([28, 80, 10]);  
    translate([2.25, 12, -2]) cube([3.5, 60, 10]);  
    translate([2.25+20, 12, -2]) cube([3.5, 60, 10]);  
    translate([14-6.5, 7-4, -2]) cylinder(d=2, h=10);
    translate([14-6.5, 7, -2]) cylinder(d=2, h=10);
    translate([14-6.5, 7+4, -2]) cylinder(d=2, h=10);
    translate([14-6.5, 7+4+4, -2]) cylinder(d=2, h=10);
    translate([14-6.5, 7+4+4+4, -2]) cylinder(d=2, h=10);
    
    translate([14+6.5, 7-4, -2]) cylinder(d=2, h=10);
    translate([14+6.5, 7, -2]) cylinder(d=2, h=10);
    translate([14+6.5, 7+4, -2]) cylinder(d=2, h=10);
    translate([14+6.5, 7+4+4, -2]) cylinder(d=2, h=10);
    translate([14+6.5, 7+4+4+4, -2]) cylinder(d=2, h=10);
    
    translate([14, 21, 5.5]) rotate([90, 0, 0]) cylinder(d=8, h=23);
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
    translate([8.5, -8, 0]) rotate([-90, 0, 0]) cylinder(d=5.9, h=25);
    translate([8.5, 3, 2]) cylinder(r=1.4, h=5);
    }
}