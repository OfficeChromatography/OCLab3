//y_slide
//an import of cart_new-DevCart.stl is needed; thus it must be in the same folder
$fn=80;

plate_holder();
//position_tip();

module plate_holder() {
difference() {
    union() {
    translate([-0.5, -190, -50])
    import("cart_new-DevCart.stl");
    translate([-60, 0, 0.5]) cube([45, 120, 7]);
    translate([22, 0, 3.5]) cube([5, 120, 4]);
    }
    translate([16, 4, 2.5]) cylinder(r=4.1, h=3.2);
    translate([-15, 4, 2.5]) cylinder(r=4.1, h=3.2);
    translate([11.9, -4, 2.5]) cube([8.2, 8.2, 3.2]);
    translate([-19.1, -4, 2.5])cube([8.2, 8.2, 3.2]);

    translate([16, 116, 2.5]) cylinder(r=4.1, h=3.2);
    translate([-15, 116, 2.5]) cylinder(r=4.1, h=3.2);
    translate([11.9, 116, 2.5]) cube([8.2, 8.2, 3.2]);
    translate([-19.1, 116, 2.5])cube([8.2, 8.2, 3.2]);
    //Noppen abschneiden
    translate([-3, 0, 7.5]) cube([6, 10, 5]);
    translate([-3, 111, 7.5]) cube([6, 10, 5]);
    //Löcher für Noppen
    translate([0, 5, 0]) cylinder(r=1.95, h=10);
    translate([0, 115, 0]) cylinder(r=1.95, h=10);
}
}

module position_tip() {
    translate([0, 0, 0]) cylinder(r1=2.7, r2=1.7, h=3);
    translate([10, 0, 0]) cylinder(r1=2.7, r2=1.7, h=3);
    translate([0, 0, -3]) cylinder(r=2, h=3);
    translate([10, 0, -3]) cylinder(r=2, h=3);
    
}