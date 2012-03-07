#!/usr/bin/perl
use lib 'blib/arch/auto/QREncode';
use QREncode;

use Data::Dumper;

$nLevel      = 1;
$nVersion    = 0;
$bAutoExtent = 1;
$nMaskingNo  = -1;

$data = 'http://sns.creyle.jp/';

$p = QREncode::CQR_Encode->new;

$res = $p->EncodeData($nLevel, $nVersion, $bAutoExtent, $nMaskingNo, $data, length $data);

print $p->swig_m_byModuleData_get, "\n";
print $p->swig_m_nSymbleSize_get, "\n";
