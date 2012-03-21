#!/usr/bin/perl
use lib 'blib/arch/auto/SQR';
use SQR;
use strict;


&main;

sub background {
    my $p = shift;
    
    $p->bg_color(0xff, 0xcc, 0xdd, 0xff);
    $p->save("background-1.png");
    
    $p->bg_image("images/jpg-cmyk.jpg", 50);
    $p->save("background-2.png");
    
    $p->bg_grad(SQR::GRADIENT::RADIAL, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff);
    $p->save("background-3.png");
}

sub mark_color {
    my $p = shift;

    $p->bg_color(0xff, 0xff, 0xff);
    
    $p->mark_color(72, 170, 72);
    $p->save("mark_color-1.png");
    
    $p->mark_color(72, 170, 72, 100);
    $p->save("mark_color-2.png");
    
    $p->mark_grad(SQR::GRADIENT::RADIAL, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff);
    $p->save("mark_color-3.png");
    
    $p->mark_image("images/jpg-cmyk.jpg");
    $p->save("mark_color-4.png");
}

## mark-type
sub mark_type {
    my $p = shift;

    $p->mark_color(72, 170, 72);
    
    $p->mark(SQR::MARKTYPE::ARC, 0.5);
    $p->save("mark_type-1.png");
   
    $p->mark(SQR::MARKTYPE::BLOCKARC, 0.5);
    $p->save("mark_type-2.png");
    
    
    $p->mark(SQR::MARKTYPE::METABALL);
    $p->shadow(3, 3, 0xaa, 0xaa, 0xaa);
    $p->save("mark_type-3.png");
}

## group 
sub group {
    my $p = shift;
    
    $p->mark(SQR::MARKTYPE::BLOCKARC, 0.5);
    
    $p->group;
    $p->group_palette_rand(0, 100);
    printf("GroupCount => %d\n", $p->group_count);
    
    $p->save("group-1.png");
    
    $p->group_image("images/0.png");
    $p->save("group-2.png");
    
    $p->group_image(undef);
    $p->save("group-3.png");
}

sub format_type {
    my $p = shift;
    
    $p->save("format-1.png");
    $p->save("format-2.pdf", SQR::FORMAT::PDF);
    $p->save("format-3.svg", SQR::FORMAT::SVG);
}

## BUG!!(perl only)
sub eye {
    my $p = shift;

    $p->eye_outer(0, 0xff, 0, 0);
    $p->eye_inner(0, 0xaa, 0, 0);
    $p->eye_image(0, 'images/0.png');
    
    $p->eye_outer(1, 0, 0xff, 0);
    $p->eye_inner(1, 0, 0xaa, 0);
    $p->eye_image(1, 'images/1.png');
    
    $p->eye_inner(2, 0, 0, 0xaa);
    $p->eye_outer(2, 0, 0, 0xff);
    $p->eye_image(2, 'images/2.png');
    
    $p->save("eye.png");
}

sub layout {
    my $p = shift;
    
    $p->layout("images/tama.png", SQR::LAYOUT::WEST);
    $p->save("layout-1.png");

    $p->paste("images/tama.png", 200, 100);
    $p->save("layout-2.png");
}

sub hole {
    my $p = SQR->new('http://creyle.jp/', SQR::LEVEL::M);
    $p->msize(16);
    
    $p->hole;
    $p->save("hole.png");
}

sub largest {
    my $p = shift;
    
    printf("x=%d y=%d w=%d h=%d\n",
        $p->largest_x,
        $p->largest_y,
        $p->largest_width,
        $p->largest_height,
    );
}

sub text {
    my $p = SQR->new('http://creyle.jp/', SQR::LEVEL::M);
    $p->msize(16);
    
    my @font = qw/
        serif
        sans-serif
        cursive
        fantasy
        monospace
    /;
    
    foreach(@font){
        $p->text($_, $_, 40, 0xff, 0, 0);
        $p->save("text-" . $_ . ".png");
    }
}

sub filter {
    my @filters = (
        { filter => 'invert',    param => [] },
        { filter => 'crayon',    param => [30.0, 3, 2, 65] },
        { filter => 'noise',     param => [2] },
        { filter => 'circle',    param => [3] },
        { filter => 'grid',      param => [8, 30] },
        { filter => 'voronoi',   param => [5] },
        { filter => 'dots',      param => [5] },
        { filter => 'hexagon',   param => [3] },
        { filter => 'diamond',   param => [3] },
        { filter => 'tile',      param => [16] },
        { filter => 'hydrangea', param => [4] },
        { filter => 'wave',      param => [10.0, 2] },
        { filter => 'ball',      param => [] },
    );
    
    foreach(@filters){
        my $p = SQR->new('http://creyle.jp/', SQR::LEVEL::M);
        $p->msize(16);
        $p->group;
        $p->group_palette_rand(0, 150);
        
        my $method = 'filter_'.$_->{filter};
        if(exists &{'SQR::'.$method}){
            $p->$method(@{$_->{param}});
            $p->save($method.'.png');
        }
    }
}

sub main {
    my $p = SQR->new('http://creyle.jp/', SQR::LEVEL::M);

    $p->msize(16);
    $p->margin(3);
    printf("ImageSize => %d\n", $p->size);
    
    background($p);
    mark_color($p);
    mark_type($p);
    group($p);
    format_type($p);
    largest($p);
    layout($p);
    eye($p);
    
    hole;
    text;
    filter;
}

1;
