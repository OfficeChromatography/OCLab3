//syringe pump spindle through motor
$fn=80;

//syringe data
//plastic syringe 3 mL!!
d1=11;          //OD syringe
r1=d1/2;         
d_head=23;     //OD syringe head
r_head=d_head/2;
h_head=2.2;  //height of syringe head
r_flange=r1; //flange under syringe head if present, else set to r1
h_flange=4;
syringe_length=60;
syringe_scale=40;  //=plunger hub max
plunger_head_d=13; //diameter of plunger head
plunger_head_r=plunger_head_d/2;
plunger_head_h=1.5; //height of plunger head
plunger_overhang=9; //mm overhang plunger when completely pressed in, without plunger head
plunger_r=4.5;

d_sensor=19.7;  //diameter of pressure sensor
r_sensor=d_sensor/2;
h_sensor=5.5;  //height of pressure sensor
y_pusher=22;  //y position of pusher when syringe is completely filled
endstop_switch=2;  //mm of the tip migrated into the endstop to be triggered

//calculations, do not change!!
y_position_endstop=23-(plunger_overhang-5+endstop_switch);
y_back=y_pusher+plunger_overhang+syringe_scale+plunger_head_h-5-5+plunger_head_h/2+5;

    //select part to render => uncomment
full_view_syringe_empty();
//full_view_syringe_full();
//full_view_syringe_half_full();
//motor();
//back();
//back_outside();
//pusher();
//syringe_holder();
//clip();
//end_stop_tip();
//syringe_half_full();

module full_view_syringe_full(){
    motor();
    back();
    translate([0, -y_back-20, 0]) syringe_holder();
    translate([-8, -y_pusher-5, 0]) pusher();  
    translate([0, -y_back-20, 0]) clip();
    translate([19.5, -(y_back+syringe_length+5), 19+10]) syringe_full();
}

module full_view_syringe_half_full(){
    motor();
    back();
    translate([0, -y_back-20, 0]) syringe_holder();
    translate([-8, -y_pusher-25, 0]) pusher();  
    translate([0, -y_back-20, 0]) clip();
    //translate([19.5, -(y_back+syringe_length+5), 19+10]) syringe_half_full();
}

module full_view_syringe_empty(){
    motor();
    back();
    translate([0, -y_back-20, 0]) syringe_holder();
    translate([-8, -y_pusher-syringe_scale-5, 0]) pusher();  
    translate([0, -y_back-20, 0]) clip();
    translate([19.5, -(y_back+syringe_length+5), 19+10]) syringe_empty();
}

module motor() {
    difference() {
    cube([39, 10+4, 39+10]);
    //holes for motor screws
    translate([4, -2, 4+10]) rotate([-90, 0, 0]) cylinder(r=1.6, h=24);
    translate([4, -2, 4+10]) rotate([-90, 0, 0]) cylinder(d=5.7, h=6);
        
    translate([4+31, -2, 4+10]) rotate([-90, 0, 0]) cylinder(r=1.6, h=24); 
    translate([4+31, -2, 4+10]) rotate([-90, 0, 0]) cylinder(d=5.7, h=6);
        
    translate([4, -2, 4+10+31]) rotate([-90, 0, 0]) cylinder(r=1.6, h=24);
    translate([4, -2, 4+10+31]) rotate([-90, 0, 0]) cylinder(d=5.7, h=6);
        
    translate([4+31, -2, 4+10+31]) rotate([-90, 0, 0]) cylinder(r=1.6, h=24);
    translate([4+31, -2, 4+10+31]) rotate([-90, 0, 0]) cylinder(d=5.7, h=6);
    //hole for spindle
    translate([19.5, -2, 19.5+10]) rotate([-90, 0, 0]) cylinder(d=7, h=24);
    //hole for motor
    translate([19.5, 7.5+4, 19.5+10]) rotate([-90, 0, 0]) cylinder(d=22.5, h=5);
    //holes for mounting
    translate([14, -3 ,5]) rotate([-90, 0, 0]) cylinder(d=5.5, h=20);
    translate([39-14, -3 ,5]) rotate([-90, 0, 0]) cylinder(d=5.5, h=20);
    translate([14, 8 ,5]) rotate([-90, 0, 0]) cylinder(d=9, h=20);
    translate([39-14, 8 ,5]) rotate([-90, 0, 0]) cylinder(d=9, h=20);
    }
}

module back() {
    difference() {
        union() {
        translate([8, -y_back, 0]) cube([23, y_back, 10]);
        //distance block to mount the syringe pumpo inside the OCLab.
    translate([8, -y_back/2-10-5, -6]) cube([23, 20, 6]);
        }
    //srew holes to mount the syringe pump (M4 screws)
    translate([24.5, -y_back/2-5, -8]) cylinder(d=4.2, h=14);
    translate([24.5, -y_back/2-5, 10-4.5]) cylinder(d=7.4, h=10);
    translate([15.5, -y_back/2-5, -8]) cylinder(d=4.2, h=14);
    translate([15.5, -y_back/2-5, 10-4.5]) cylinder(d=7.4, h=10);
    
    //srew holes to mount the motor holder (module motor)
    translate([14, -12 ,5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
    translate([39-14, -12 ,5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
        
    //screw holes to mount the syringe holder
    translate([14, -y_back, 5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
    translate([39-14, -y_back ,5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
    //slider cutout
    translate([8, -95, 5]) rotate([-90, 0, 0]) cylinder(r=2.1, h=100);
    translate([39-8, -95, 5]) rotate([-90, 0, 0]) cylinder(r=2.1, h=100);
    }
}

module back_outside() {
    difference() {
    translate([8, -y_back, 0]) cube([23, y_back, 10]);
    //srew holes to mount the syringe pump ouside the electronic box (M4 screws)
    translate([24.5, -y_back+20, -8]) cylinder(d=4.2, h=16);
    translate([24.5, -y_back+20, 10-4.5]) cylinder(d=7.4, h=10);
    translate([15.5, -y_back+50, -8]) cylinder(d=4.2, h=16);
    translate([15.5, -y_back+50, 10-4.5]) cylinder(d=7.4, h=10);
    
    //srew holes to mount the motor holder (module motor)
    translate([14, -12 ,5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
    translate([39-14, -12 ,5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
        
    //screw holes to mount the syringe holder
    translate([14, -y_back, 5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
    translate([39-14, -y_back ,5]) rotate([-90, 0, 0]) cylinder(d=4.8, h=15);
    //slider cutout
    translate([8, -95, 5]) rotate([-90, 0, 0]) cylinder(r=2.1, h=100);
    translate([39-8, -95, 5]) rotate([-90, 0, 0]) cylinder(r=2.1, h=100);
    }
}

module syringe_holder() {
    difference () {
        union() {
        translate([0, 0, 0]) cube([39, 20, 19+10]);
        translate([0, 0, 19+10]) cube([(39-d1)/2, 20, d1/2]);
        translate([0, -9, 19+8-r1]) cube([39, 10, 7.5]);
        //end stop housing
        translate([36, 6, 0]) cube([12, 14, 40]);
        }
    //syringe cutout
    translate([19.5, -10, 19+10]) rotate([-90, 0, 0]) cylinder(r=r1, h=40, $fn=80);
    translate([19.5, 15, 19+10]) rotate([-90, 0, 0]) cylinder(r=r_head, h=h_head, $fn=80);
    translate([19.5-r_head, 15, 19+10]) cube([20, h_head, 19]);
    translate([19.5, 15-h_flange, 19+10]) rotate([-90, 0, 0]) cylinder(r=r_flange, h=h_flange, $fn=80);
    translate([19.5-r_flange, 15-h_flange, 19+10]) cube([20, h_flange, 19]);
    //srew hole for clip 
    translate([6.75, 10, 19+8]) cylinder(r=1.4, h=15);
    //end stop cutout
    translate([39, 8, 2]) cube([10, 14, 34]);
    translate([39, 4, 8]) cube([10, 14, 13]);
    translate([39, 4, 30]) cube([10, 14, 6]);
    translate([39+5, 4, 4.5]) rotate([-90, 0, 0]) cylinder(d=3.6, h=10);
    translate([39+5, 4, 25]) rotate([-90, 0, 0]) cylinder(d=3.6, h=10);
    //srew holes for mounting (M5)
    translate([14, -2 ,5]) rotate([-90, 0, 0]) cylinder(d=5.5, h=25);
    translate([39-14, -2 ,5]) rotate([-90, 0, 0]) cylinder(d=5.5, h=25);
    translate([14, -2 ,5]) rotate([-90, 0, 0]) cylinder(d=8.6, h=12);
    translate([39-14, -2 ,5]) rotate([-90, 0, 0]) cylinder(d=8.6, h=12);
    }
}

module clip() { //clip to fix the syringe
    difference() {
        union() {
    translate([0, -0, 19+10+r1]) cube([38-13.5, 15, 5]);
    translate([38-13.5, 7.5, 19+10+r1]) cylinder(d=15, h=5, $fn=80);
        }
    translate([4.75, 10, 19+10]) cylinder(r=1.6, h=15);
    }
}

module pusher() {
    difference() {
        union() {
            translate([8, 0, 0]) cube([39, y_pusher, 38]);
            //end stop tip
            translate([48, y_position_endstop, 13.5]) end_stop_tip();
        }
        //back cutout
        translate([16-0.1, -10, 0]) cube([23.2, 120, 10.2]);
        //syringe plunger cutout
        translate([19.5+8, -10, 19+10]) rotate([-90, 0, 0]) cylinder(r=plunger_r, h=20, $fn=80);
        translate([19.5+8-plunger_r, 0, 19+10]) cube([plunger_r*2, 5, 20]);
        //plunger head cutout
        translate([19.5+8, 5, 19+10]) rotate([-90, 0, 0]) cylinder(r=plunger_head_r, h=plunger_head_h, $fn=80);
        translate([19.5+8-plunger_head_r, 5, 19+10]) cube([plunger_head_r*2, plunger_head_h, 20]); 
        //pressure sensor cutout
        translate([19.5+8, 5+plunger_head_h, 19+10]) rotate([-90, 0, 0]) cylinder(r=r_sensor, h=h_sensor);
        translate([-10, 5+plunger_head_h, 19+10-r_sensor]) cube([10+19.5+8, h_sensor, r_sensor*2]);
        //hole to press out the sensor
        translate([0, 5+h_sensor-plunger_head_h, 19+10]) rotate([0, 90, 0]) cylinder(d=4.9, h=50);
        
        //spindle head cutout
        r_spindle_head=2;
        h_spindle_head=6.2;
        d_spindle=5;
        r_spindle=d_spindle/2;
        
    //spindle cutout
        translate([19.5+8, 22-h_spindle_head-1, 19+10]) rotate([-90, 0, 0]) cylinder(r=r_spindle_head, h=h_spindle_head);
        translate([19.5+8, 22-h_spindle_head+2, 19+10]) rotate([0, 0, 0]) cylinder(d=3.8, h=10);
        translate([19.5+8, 22-2, 19+10]) rotate([-90, 0, 0]) cylinder(r=r_spindle, h=5);
    //holes to move in motor srews
    translate([4+8, y_pusher-6, 4+10]) rotate([-90, 0, 0]) cylinder(d=6.5, h=8);
    translate([4+31+8, y_pusher-6, 4+10]) rotate([-90, 0, 0]) cylinder(d=6.5, h=8); 
    }
    //slider
    translate([16, 0, 4.8]) rotate([-90, 0, 0]) cylinder(r=2, h=22);
    translate([39, 0, 4.8]) rotate([-90, 0, 0]) cylinder(r=2, h=22);
}

module syringe_full() {
    translate([0, 0, 0]) rotate([-90, 0, 0]) cylinder(r=r1, h=syringe_length);
    translate([0, -4, 0]) rotate([-90, 0, 0]) cylinder(r=r1-1, h=syringe_length);
    translate([0, syringe_length, 0]) rotate([-90, 0, 0]) cylinder(r=r_head, h=h_head);
    
    translate([0, syringe_length+h_head, 0]) rotate([-90, 0, 0]) cylinder(r=plunger_r, h=plunger_overhang+syringe_scale);
    translate([0, syringe_length+h_head+plunger_overhang+syringe_scale, 0]) rotate([-90, 0, 0]) cylinder(r=plunger_head_r, h=plunger_head_h);
}

module syringe_half_full() {
    translate([0, 0, 0]) rotate([-90, 0, 0]) cylinder(r=r1, h=syringe_length);
    translate([0, -4, 0]) rotate([-90, 0, 0]) cylinder(r=r1-1, h=syringe_length);
    translate([0, syringe_length, 0]) rotate([-90, 0, 0]) cylinder(r=r_head, h=h_head);
    
    translate([0, syringe_length+h_head, 0]) rotate([-90, 0, 0]) cylinder(r=plunger_r, h=plunger_overhang+syringe_scale/2);
    translate([0, syringe_length+h_head+plunger_overhang+syringe_scale/2, 0]) rotate([-90, 0, 0]) cylinder(r=plunger_head_r, h=plunger_head_h);   
}

module syringe_empty() {
    rotate([-90, 0, 0]) cylinder(r=r1, h=syringe_length);
    translate([0, -4, 0]) rotate([-90, 0, 0]) cylinder(r=r1-1, h=syringe_length);
    translate([0, syringe_length, 0]) rotate([-90, 0, 0]) cylinder(r=r_head, h=h_head);
    translate([0, syringe_length+h_head, 0]) rotate([-90, 0, 0]) cylinder(r=plunger_r, h=plunger_overhang);
    translate([0, syringe_length+h_head+plunger_overhang, 0]) rotate([-90, 0, 0]) cylinder(r=plunger_head_r, h=plunger_head_h);
}

module end_stop_tip() {
    difference() {
        union() {
        translate([-6, 5, 0]) rotate([0, 0, -90]) cube ([18, 14.5, 4.0]);
        translate([0, 0, 1]) rotate([0, 0, -90])
cube ([20.0, 7.5, 2.0]);
        }
    translate([0, -10, 4]) rotate([0, 8, -90])
cube ([11, 7.5, 1.0]);
    translate([0, -10, -1]) rotate([0, -8, -90])
cube ([11.0, 7.5, 1.0]);
    }
}