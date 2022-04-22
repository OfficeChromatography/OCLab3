//y_cart
//an import of cart_new-DevCart.stl is needed; thus it must be in the same folder
$fn=80;

y_cart_enlarged();

module y_cart_enlarged() {
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

    translate([-3, 0, 7.5]) cube([6, 10, 5]);
    translate([-3, 111, 7.5]) cube([6, 10, 5]);
    //cutout for M4 screw fitting to the positioning holes of the plate holder
    translate([0, 5, 0]) cylinder(r=1.95, h=10);
    translate([0, 115, 0]) cylinder(r=1.95, h=10);
    //cutout pulley
    #translate([-69, 50, -1]) cube([20, 20, 5]);
}
}