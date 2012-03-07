#!/usr/bin/perl
use SQR;

my $p = SQR->new('http://creyle.jp/', SQR::LEVEL::L);
$p->msize(4);
#$p->mark_color(72, 170, 72);

#$p->mark_grad(SQR::GRADIENT::L2R, 0x0, 0, 0, 0xff, 0, 0xff);


$p->mark(SQR::MARKTYPE::BLOCKARC);
#$p->shadow(3, 3, 0xaa, 0xaa, 0xaa, 0xff);


$p->mark_image("icon.png", 0xff);


#$p->layout("icon.png", SQR::LAYOUT::CENTER);
$p->save("test2.png");


