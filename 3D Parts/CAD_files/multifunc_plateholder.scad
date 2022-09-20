//multi-functional plate holder
$fn=80;

plate_holder(); //glass strip plus 3 mm silicone mat
//translate([0, -2, 7.5]) cover_development();  //plate cover during development
//cutting_aid();  //for silicone mat

x=128;
y=124;
h=10;
platte_x=101;
platte_x_50=104;
platte_y=101;
platte_y_50=51;

module plate_holder() {   //glass strip+3 mm silicone mat
    color( "Gray", 1.0 ) {
    difference() {
    union() {
        main_body();            
        //position burling for aluminum plate
        translate([-50, -1, -3]) cylinder(r=1.5, h=3, $fn=60);
        //pressure springs
        translate([-20, -54, 0.2]) cylinder(r=2.5, h=3.3, $fn=60);
        translate([20, -54, 0.2]) cylinder(r=2.5, h=3.3, $fn=60);
        translate([-platte_x/2, -56.5, -1.5]) cube([30.5, 2, 5]);
        translate([platte_x/2-30.5, -56.5, -1.5]) cube([30.5, 2, 5]);
    }
    //lowering for glass+silicone mat: h=3,9 mm
    translate([-57, 48.5, -2]) cube([platte_x+13.5, 9.2, 10]);
    //cut to retain front or back part: select
        //front part
    //translate([-65, -62, -5]) cube([130, 14.5, 10]);
        //back part
    translate([-65, -47.5, -5]) cube([130, 110, 10]);
}
}
//HPTLC plate for testing
//translate([-platte_x/2, -platte_y/2-2, 0]) cube([platte_x, platte_y, 1.2]);

//aluminum plate for testing
//translate([-platte_x/2, -platte_y/2+3, -2]) cube([platte_x, platte_y-5, 2]); 

//glas strip+silicone mat for testing
//translate([-57, 48.5, -2.5]) cube([114, 10, 3.9]);
}

module main_body() {
    color( "Gray", 1.0 ) {
    difference() {
        //main body
        cube([x, y, h], center=true);
        //cutout plate
        translate([0, -2, 2]) cube([platte_x, platte_y, h-2], center=true);
        //cutout slit
        translate([-51.5, -50, -2]) cube([1, 10, 10]);
        translate([50.5, -50, -2]) cube([1, 10, 10]);        
        //pressure springs cutout
        translate([-platte_x/2, -(platte_y/2+8), -2]) cube([platte_x, 10, 10]);
        //lowering heating mats
        translate([0, -7, 0]) cube([platte_x, 66, 5.0], center=true);
        //cutout cover plate
        translate([0, 0, 4.3]) cube([platte_x+15, platte_y+15+4, 1.4], center=true);
    //chambers for round magnets
        translate([60, 23+15-7, -2]) cube([10, 8, 3.1], center=true);
        translate([60, 23-16-6, -2]) cube([10, 8, 3.1], center=true);
        translate([-60, 23+15-7, -2]) cube([10, 8, 3.1], center=true);
        translate([-60, 23-16-7, -2]) cube([10, 8, 3.1], center=true);
    //holes under round magnets
        translate([55, 23+15-7, -7]) cylinder(r=2, h=4);
        translate([55, 23-16-6, -7]) cylinder(r=2, h=4);
        translate([-55, 23+15-7, -7]) cylinder(r=2, h=4);
        translate([-55, 23-16-7, -7]) cylinder(r=2, h=4);
    //positon holes: M4 screw
        distance=111.0;
        rand=(x-distance)/2;
        translate([x/2-rand, 16, -6]) cylinder(r=3.75, h=6);
        mirror([1,0,0]) translate([x/2-rand, 16, -6]) cylinder(r=3.75, h=6);
        //heating mat contacts
        translate([23, -6, -2]) cube([55, 20, 5], center =true);
        //heating mat cables
        translate([55, -9, -2]) cube([30, 12, 4], center=true);
        //thermistor
        translate([20, -38, -2]) cube([61, 4, 4], center=true);
        //thermistor cables
        translate([58, -38, -3]) cube([15, 2*2.7, 2.7], center=true);
        //slit for glass strip
        translate([-55, -46.5, -2]) cube([110, 1.6, 10]);
       //magnet cubes back part
        translate([-60, -50.1, -1]) cube([5.2, 5.2, 5.4], center=true);
        translate([60, -50.1, -1]) cube([5.2, 5.2, 5.4], center=true);
        //magnet cubes front part
        translate([-60, -45.1, -1]) cube([5.2, 5.2, 5.4], center=true);
        translate([60, -45.1, -1]) cube([5.2, 5.2, 5.4], center=true);
        //holes under magnet cubes
        translate([-60, -52, -6]) cylinder(r=2, h=3, $fn=60);
        translate([60, -52, -6]) cylinder(r=2, h=3, $fn=60);
        translate([-60, -42, -6]) cylinder(r=2, h=3, $fn=60);
        translate([60, -42, -6]) cylinder(r=2, h=3, $fn=60);
        }  
    }        
}

module cover_development() {
    color( "Gray", 1.0 ) {
    difference() {
    cube([platte_x+14.8, platte_y+14.8+8, 8], center=true);
    translate([0, 0, 3.5]) cube([platte_x+6, platte_y+6, 1], center=true);
    translate([0, 0, 0]) cube([platte_x, platte_y, 12], center=true);
    //cut back
    translate([-65, -62, -5]) cube([130, 16.5, 10]);
    //slit for glass strip
    translate([-55, -44.5, -4]) cube([110, 1.6, 8]);
    translate([-50.5, -50, -5]) cube([101, 6, 10]);    
    }
//glass plate cover for testing
    //translate([0, 4, 3.5]) cube([platte_x, platte_y-7, 1], center=true);
//glass strip for testing
    //translate([-55, -44.5, -6]) cube([110, 1.6, 12]);    
}
}

module cutting_aid() {   //silicone mat
k=0.8; //knife thickness
HF=9; //mat height
LF = 114; //mat length
DF = 2.9; //mat thickness
difference() {
cube([150, 60, 10]);
translate([20, 0, 10-DF]) cube([LF, 28, 10]); 
translate([10, 28-HF-k, 0]) cube([135, k, 10]); 
string = str(LF, " x ", HF, " x ", DF, " mm");
   # translate ([12, 35, 7])
   # linear_extrude(4) text(string, size = 6, direction = "ltr", spacing = 1 );     
}
}