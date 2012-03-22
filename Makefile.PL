use ExtUtils::MakeMaker;
WriteMakefile(
	CC      => 'gcc',
	LD      => 'g++',
	NAME    => 'SQR',
	DEFINE  => '-DQDR_SWIG_PERL',
	INC     => '-I/usr/include/cairo -Ipikl -I.',
	LIBS    => '-lcairo -ljpeg -lgif -lpng -L/lib/gcc/i686-pc-cygwin/4.5.3 -lstdc++',
	OPTIMIZE => '-O3',
	OBJECT  => q|
        build/perl/sqr_wrap.o
        encode/QR_Encode.o
        draw/qdr_draw.o
        draw/qdr_largest.o
        draw/qdr_filter.o
        draw/qdr_stack.o
        draw/qdr_version.o
        draw/qdr_group.o
        draw/qdr_mark.o
        draw/qdr_eye.o
        draw/qdr_text.o
        draw/qdr_paste.o
        draw/qdr_import.o
        draw/qdr_test.o
        pikl/pikl_io.o
        pikl/pikl_pixel.o
        pikl/pikl_jpeg.o
        pikl/pikl_png.o
        pikl/pikl_bitmap.o
        pikl/pikl_rotate.o
        pikl/pikl_resize.o
        pikl/pikl_enhance.o
        pikl/pikl_affine.o
        pikl/pikl_effect.o
        pikl/pikl_pattern.o
        pikl/pikl_blur.o
        pikl/pikl_decrease.o
        pikl/pikl_divide.o
        pikl/pikl_composite.o
        pikl/pikl_camera.o
        pikl/pikl_special.o
        pikl/pikl_voronoi.o
        pikl/pikl_scrap.o
        pikl/pikl_algorithm.o
        pikl/pikl_noise.o
        pikl/pikl_draw.o
    |,
    clean => {
        FILES => "pikl/*.o draw/*.o encode/*.o build/perl/*.o *.png *.pdf *.svg"
    },
);

package MY;
sub c_o {
    my $src = shift->SUPER::c_o(@_);
    my ($from, $to) = ('$(DEFINE) $*.', '$(DEFINE) -o $@ $*.');
    $src =~ s{\Q$from\E}{$to}gxms;
    return $src;
}

