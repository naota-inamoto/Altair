#!/usr/bin/env perl

use encoding 'euc-jp';
binmode STDERR, ':encoding(euc-jp)';

######################################################################
#
#		 KNP�η����ǡ�ʸ��롼���translator
#
#					99/09/10 by kuro@i.kyoto-u.ac.jp
#					00/02/15 last modified
######################################################################
#
# �ƹԤ�notation
# ==============
#	[����ʸ̮]��ʬ����[���ʸ̮]\t+FEATURE��
#		�� FEATURE�������ɬ��TAB�Ƕ��ڤ�
#		�� �����ʸ̮����ʬ���Ȥ����ʸ�ᤴ�Ȥ˶���Ƕ��ڤ�
#						(ʸ��롼��ξ��)
#		�� FEATURE���FEATURE���Ȥ˶���Ƕ��ڤ�
#
#		�� ʸ��롼��ξ�������ʸ̮�����ȸ��ʸ̮�θ��
#		   Ǥ�դ�ʸ��������(?*����ư���������)
#		�� �����ǥ롼��ξ�������ʸ̮�����ȸ��ʸ̮�θ��
#		   Ǥ�դη�����������(?*����ư���������)
#
# �����ǡ�ʸ���notation
# ======================
#	^....		ʸ����Ƭ����ޥå� (ʸ��롼��ξ��)
#	<<...>>		ʸ���feature
#	<...>		ľ���η����Ǥ�feature (��<...>�Ȥ����feature�����λ���)
#
#	(...)		0��ʾ�νи� (�����ǥ�٥��ʸ���٥뤽�줾��)
#	{WORD1|WORD2|..}WORD1 or WORD2 or ...
#			  (������Τ��ʻ줬Ʊ��ξ�����)
#
#	{\�ʻ�}		�����ʻ�
#	{\�ʻ�:��ʬ��}	���κ�ʬ��
#
#	��		Ǥ�դη������� (�� �ġĤϵ�����ǻȤäƤ���)
#	��		Ǥ�դη����� (�����ǲ��ϤΤ�����������롼��ǤϺ��)
#
#	WORD_���Ѹ�G	Ǥ�դγ��Ѹ� (Ǥ�դγ��ѷ�)
#	WORD_����Ѹ�G	Ǥ�դ�����Ѹ�
#
#	WORD_���Ѹ�g	�ʻ�,��ʬ��:*�����ѷ�:�ʲ��ΰ��̲������ѷ�:���Τޤޡ���:*
#			  ư�쥿���� ��) ��g
#                         --------------------
#				�첻ư�� �Ҳ�ư����� ����ư�� ����ư�� ����ư��
#				ư�����������ޤ��� ư�������������뷿 ư�������������뷿
#				̵���ѷ�
#			  �����ƻ쥿���� ��) ������g
#                         -------------------------- 
#			 	�����ƻ����
#				��ư��̷� ��ư�줯��
#			  �ʷ��ƻ쥿���� ��) �Ť���g
#                         --------------------------
#				�ʷ��ƻ� �ʷ��ƻ��ü� �ʥη��ƻ� Ƚ���
#				��ư��̷� ��ư������� ��ư�줽������
#				(�����̤Ρפˤ���Х����ü�Ϣ�ΤȤʤ�)
#				(�� �ֽ�ư��̷��פϥ����ƻ�ȥʷ��ƻ�ξ��)
#			  ������ƻ쥿���� ��) Ʋ������g
#                         ------------------------------
#				������ƻ�
#
#	WORD_����Ѹ�g	̾�졤̾����̾����������̾�����Ҹ�������
#
#	WORD12345	�ʻ졤��ʬ�ࡤ���ѷ������ѷ�����ˤĤ��ƿ����������ʬ�����Τ���
#			  ��) ����1235 --> ���ѷ��������Ǥ�褤
#
#	���㳰���ࢨ	��"#define WORD �̲�ͽ���"�Ȥ�������WORDg�Ȳ��
#			������������̤�g�ʤ��Ǥ��ΰ��̲���Ԥ�
#			��̾�졤���졤����Ͼ�˺�ʬ�����̲�

# ���ѷ����б��դ�
# -----------------------------------------
# ̵���ѷ�		ư��
# ��ư��̷�		�����ƻ� �ʷ��ƻ�
# ��ư�줯��		�����ƻ�
# ư�����������ޤ���	ư��
# ư�������������뷿	ư��
# ư�������������뷿	ư��

$conj_type[0] = "�첻ư�� �Ҳ�ư�쥫�� �Ҳ�ư�쥫��¥���ط� �Ҳ�ư�쥬�� �Ҳ�ư�쥵�� �Ҳ�ư�쥿�� �Ҳ�ư��ʹ� �Ҳ�ư��й� �Ҳ�ư��޹� �Ҳ�ư���� �Ҳ�ư���ԥ��� �Ҳ�ư���� �Ҳ�ư����ʸ�첻�ط� ����ư�� ����ư���� ����ư�� ����ư�� ư�����������ޤ��� ư�������������뷿 ư�������������뷿 ̵���ѷ�";
$conj_type[1] = "�����ƻ쥢������ �����ƻ쥤�� �����ƻ쥤���ü� ��ư��̷� ��ư�줯��";
$conj_type[2] = "�ʷ��ƻ� �ʷ��ƻ��ü� �ʥη��ƻ� Ƚ��� ��ư��̷� ��ư������� ��ư�줽������";
$conj_type[3] = "������ƻ�";

# ���������Τ˽񤤤Ƥ��������褤�����Ȥꤢ�����Ȥ����Τ���

$pos_repr{"\\̾��:����̾��"} = "����";
$pos_repr{"\\̾��:����Ū̾��"} = "����";
$pos_repr{"\\������"} = "��";
$pos_repr{"\\������:̾����̾�������"} = "��";
$pos_repr{"\\������:̾�����ü�������"} = "����";
$pos_repr{"\\����"} = "����";
$pos_repr{"\\����:�ʽ���"} = "��";
$pos_repr{"\\����:������"} = "����";
$pos_repr{"\\�ؼ���:̾����ֻؼ���"} = "����";
$pos_repr{"\\�ؼ���:������ֻؼ���"} = "����";
$pos_repr{"\\Ϣ�λ�"} = "�ۤ��";
$pos_repr{"\\�ؼ���:Ϣ�λ���ֻؼ���"} = "����";
$pos_repr{"\\��³��"} = "������";
$pos_repr{"\\��ư��"} = "����";
$pos_repr{"\\�ü�:����"} = "��";

######################################################################
use KNP;
$knp = new KNP(-Option => "-bnst -tab");
######################################################################
$bnstrule_flag = 1;
$num = 0;

while ( <STDIN> ) {
    
    chomp;
    $num ++;
    next if (/^[\s\t]*\;/ || length($_) == 0);

    if (/^\#define/) {
	@tmp = split;
	if ($tmp[1] eq "ʸ��롼��") {
	    $bnstrule_flag = 1;
	} 
	elsif ($tmp[1] eq "�����ǥ롼��") {
	    $bnstrule_flag = 0;
	} 
	elsif ($tmp[2] eq "�̲�ͽ���") {
	    $g_define_word{$tmp[1]} = 1;
	} 
	else {
	    die "Invalid define line ($_)!!\n" 
	}
	next;
    }


    # ɽ����FEATURE��ȥ����Ȥ�ʬΥ
    # 	FEATURE��Ȥδ֤�tab�������Ȥ� ; �θ�

    if (/^([^\t]+)[\s\t]+([^\;\t]+)[\s\t]*(\;.+)$/) {
	$pattern = $1; $feature = $2; $comment = $3;
    } elsif (/^([^\t]+)[\s\t]+([^\;\t]+)[\s\t]*$/) {
	$pattern = $1; $feature = $2; $comment = "";
    } else {
	print STDERR  "line $num is invalid; $_\n";
	next;
    }
    $pattern =~ s/^\s+//;	# ǰ�Τ���
    $pattern =~ s/\s+$//;	# ǰ�Τ���


    # �����ʸ̮�ȼ�ʬ���Ȥ�ʬΥ�������ʸ̮��[...]

    $pattern =~ /^(\[[^\[\]]+\])?([^\[\]]+)(\[[^\[\]]+\])?$/;
    $tmp1 = $1; $tmp2 = $2; $tmp3 = $3;
    $tmp1 =~ s/^\[|\]$//g;
    @pres = split(/ /, $tmp1);
    $tmp2 =~ s/^\[|\]$//g;
    @self = split(/ /, $tmp2);
    $tmp3 =~ s/^\[|\]$//g;
    @poss = split(/ /, $tmp3);

    # print ">> pre  @pres\n self @self\n post @poss\n\n";

    # �����ʸ̮������ˤ϶�ʸ���������(���ϻ���?*)

    if ($bnstrule_flag) {
	unshift(@pres, "") if ($pres[0] ne "��");
	push(@poss, "") if ($poss[$#poss] ne "��");
    } else {
	unshift(@pres, "") if (!@pres);
	push(@poss, "") if (!@poss);
    }	

    @all = (@pres, @self, @poss);

    # ��ɽ���Ĥ���

    undef @repr_str;
    foreach (@all) {
	push(@repr_str, bnst_cond($_, 0));
    }

    # MAIN

    print "; $pattern\n";
    print "(\n(";
    # ����ʸ̮
    for ($i = 0; $i < @pres; $i++) {
	if ($bnstrule_flag) {
	    if ($i == 0) {
		print " ?*";
	    } else {
		bnst_cond($all[$i], 1, $repr_str[$i-1], $repr_str[$i+1]);
	    }
	}
	else {
	    print " ?*";
	    bnst_cond($all[$i], 1, "", $repr_str[$i+1]);
	}
    }	
    print " )\n(";
    # ��ʬ
    for ($i = @pres; $i < @pres + @self; $i++) {
	bnst_cond($all[$i], 1, $repr_str[$i-1], $repr_str[$i+1]);
    } 
    print " )\n(";
    # ���ʸ̮
    for ($i = @pres + @self; $i < @all ; $i++) {
	if ($bnstrule_flag) {
	    if ($i == (@all - 1)) {
		print " ?*";
	    } else {
		bnst_cond($all[$i], 1, $repr_str[$i-1], $repr_str[$i+1]);
	    }
	}
	else {
	    bnst_cond($all[$i], 1, $repr_str[$i-1], "");
	    print " ?*";
	}
    }
    print " )\n\t$feature RID:$num\n)\n";
    print "$comment\n" if $comment;
    print "\n";
}

# $juman->close;
# $knp->close;

######################################################################
sub bnst_cond
{
    # ʸ��ξ������ (flag=1:�̾�ν�����flag=0:��ɽɽ���򤫤���)

    my ($input, $flag, $l_context, $r_context) = @_;
    my ($ast_flag, $string, $feature);

    # (....) -> ....��ast_flag��ʬΥ

    if ($bnstrule_flag && $input =~ /^\((.+)\)$/) {
	$input = $1;
	$ast_flag = 1;
    } else {
	$ast_flag = 0;
    }


    # ʸ���FEATURE��ʬΥ

    if ($input =~ /^\<\<([^\<\>]+)\>\>$/) {
	$string = "��"; $feature = $1;
    } elsif ($input =~ /^(.+)\<\<([^\<\>]+)\>\>$/) {
	$string = $1; $feature = $2;
    } else {
	$string = $input; $feature = "";
    }


    # ����

    if ($flag) {
	if ($bnstrule_flag) {
	    print " < (";
	    bnst_cond2($string, 1, $l_context, $r_context);
	    printf " )%s >", feature2str($feature);
	    print "*" if $ast_flag;
	} else {
	    bnst_cond2($string, 1, $l_context, $r_context);
	}
    } else {
	return bnst_cond2($string, 0);
    }
}

######################################################################
sub feature2str
{
    my ($input) = @_;

    return "" unless ($input);

    $data = " (";
    foreach (split(/\|\|/, $input)) {
	s/\&\&/ /g;
	$data .= "($_)";
    }
    $data .= ")";
    return $data;
}

######################################################################
sub bnst_cond2
{
    # ʸ��ξ������ (flag=1:�̾�ν�����flag=0:��ɽɽ���򤫤���)

    # (��|���)��(����|����|����)��
    #
    # part: [0][0]:��   [1][0]:�� [2][0]:���� [3][0]:��
    #       [0][1]:���           [2][1]:����
    #                             [2][1]:����
    # 
    # data[0][0]{phrase}: ��ˤ�����
    # data[0][1]{phrase}: ����ˤ�����
    # data[2][1]{phrase}: ��ˤ����
    # data[2][2]{phrase}: ��ˤ�����
    # 
    # �� data[0][0]{phrase}�Ϥ��٤Ƥ�part��0���ܤθ�򽸤᤿���
    #    data[i][j]{phrase}��i���ܤ�part��j���ܤθ�ˤ������

    my ($input, $flag, $l_context, $r_context) = @_;
    my (@data, @feature, @mrph);
    my ($i, $j, $k, $l, $error_flag, $any_prefix, $knp_input);

    # ��Ƭ�� ^ �Ǥ������Ƭ����ξ��

    if ($input =~ s/^\^// || $bnstrule_flag == 0) {
	$any_prefix = 0;
    } else {
	$any_prefix = 1;
    }

    # ���Ǥ��Ȥ�ʬ�� --> @part
    # (���,�Ť����塤>,G,g,�����θ�˶���������)

    $input =~ s/\(/ \(/g;
    $input =~ s/\)/\) /g;
    $input =~ s/\{/ \{/g;
    $input =~ s/\}/\} /g;
    $input =~ s/��/ �� /g;
    $input =~ s/\>/\> /g;
    $input =~ s/G/G /g;
    $input =~ s/g/g /g;

    $input =~ s/(\d+)/\1 /g;
    $input =~ s/(\d) (\))/\1\2/g;
    $input =~ s/(\d) (\<)/\1\2/g;

    $input =~ s/^ +| +$//g;
    @part_str = split(/ +/, $input);
    $part_num = @part_str;

    # @part_str����������@part(2��������)�����

    for ($i = 0; $i < $part_num; $i++) {
	if ($part_str[$i] =~ /^\((.+)\)$/) {
	    $part_str[$i] =  $1;
	    $feature[$i]{ast} = 1;
	}
	elsif ($part_str[$i] =~ /^\{(.+)\}$/) {
	    $part_str[$i] =  $1;
	}

	# OR�ξ���ɽ��ɽ������
	if ($part_str[$i] !~ /^\<\</ && $part_str[$i] =~ /\|/) {
	    @{$part[$i]} = split(/\|/, $part_str[$i]);
	    $feature[$i]{or} = 1;
	}

	# ¾�ϼ�Υ᥿ɽ�����θ
	else {
	    @{$part[$i]} = ($part_str[$i]);

	    if ($part[$i][0] =~ /\<(.+)\>$/) {
		$feature[$i]{lastfeature} = feature2str($1);
		$part[$i][0] =~ s/\<.+\>$//;
	    }
	    if ($part[$i][0] eq "��") {
		$feature[$i]{result} = " ?*";
		$part[$i][0] = "��";
	    }
	    if ($part[$i][0] =~ /G$/) {
		$feature[$i]{lastGENERAL} = 1;
		$part[$i][0] =~ s/G$//;
	    }
	    if ($part[$i][0] =~ /g$/) {
		$feature[$i]{lastgeneral} = 1;
		$part[$i][0] =~ s/g$//;
	    }
	    if ($part[$i][0] =~ /([\d]+)$/) {
		$feature[$i]{lastnum} = $1;
		$part[$i][0] =~ s/[\d]+$//;
	    }
	    if ($part[$i][0] =~ /^\\/) {
		$feature[$i]{result} = " [$part[$i][0]]";
		$feature[$i]{result} =~ s/\\//;
		$feature[$i]{result} =~ s/\:/ /;
		$part[$i][0] = $pos_repr{$part[$i][0]};
	    }
	}
    }

    # $part[$i][0]��ɸ��ǡ����Ȥ���$data[0][0]��

    for ($i = 0; $i < $part_num; $i++) {
	$data[0][0]{phrase} .= $part[$i][0];
	$data[0][0]{length}[$i] = length($part[$i][0]);
    }

    # ��ɽɽ�����֤����

    return $data[0][0]{phrase} if ($flag == 0);

    # i���ܤ�part��j���ܤθ�ˤ�����Τ�$data[i][j]��

    for ($i = 0; $i < $part_num; $i++) {
	next unless ($feature[$i]{or});
	for ($j = 1; $j < @{$part[$i]}; $j++) {
	    for ($k = 0; $k < $part_num; $k++) {
		if ($k != $i) {
		    $data[$i][$j]{phrase} .= $part[$k][0];
		    $data[$i][$j]{length}[$k] = length($part[$k][0]);
		} else {
		    $data[$i][$j]{phrase} .= $part[$k][$j];
		    $data[$i][$j]{length}[$k] = length($part[$k][$j]);
		}
	    }
	}
    }

    # ���줾��KNP(JUMAN)�ǽ���

    for ($i = 0; $i < $part_num; $i++) {
	for ($j = 0; $j < @{$part[$i]}; $j++) {
	    next unless ($data[$i][$j]);

	    # ������"��|��"�ʤ�"��"������ʳ��ʤ�"����"����Ϳ
	    # �� �ʲ���������褹�뤿��
	    #      1. Ϣ�η���ʸ�᤬��������Ǥ�������JUMAN����ʤ�
	    #      2. ñ��"��"����Ϳ�����"Ʊ��"���촴�ˤʤ�
	    #      2. "����"��JUMAN��̤�����
	    #      3. ������"��"��JUMAN��̤�����

	    $knp_input = $l_context . $data[$i][$j]{phrase} . $r_context;

	    if($bnstrule_flag){
		if ($knp_input =~ /(��|��|��|��)$/) {
		    $knp_input .= "��";
		} else {
		    $knp_input .= "����";
		}
	    }

	    $knp->parse($knp_input); 
	    @knp_result = split(/\n/, $knp->{ALL});

	    # print "\n\n>>>>>$knp_input\n>>>@knp_result\n";

	    $k = 0;
	    foreach $item (@knp_result) { 
		next if ($item =~ /^EOS/);
		# next if ($item =~ /^\@/);
		next if ($item =~ /^(\*|\#|\;)/);
		($mrph[$i][$j][$k]{word}, 
		 $mrph[$i][$j][$k]{yomi}, 
		 $mrph[$i][$j][$k]{base}, 
		 $mrph[$i][$j][$k]{pos}, 
		 $mrph[$i][$j][$k]{d1}, 
		 $mrph[$i][$j][$k]{pos2}, 
		 $mrph[$i][$j][$k]{d2}, 
		 $mrph[$i][$j][$k]{conj}, 
		 $mrph[$i][$j][$k]{d3}, 
		 $mrph[$i][$j][$k]{conj2}, 
		 $mrph[$i][$j][$k]{d4}) = split(/ /, $item);
		$mrph[$i][$j][$k]{length} = length($mrph[$i][$j][$k]{word});
		$mrph[$i][$j][$k]{result} = 
		    " [$mrph[$i][$j][$k]{pos} $mrph[$i][$j][$k]{pos2} $mrph[$i][$j][$k]{conj} $mrph[$i][$j][$k]{conj2} $mrph[$i][$j][$k]{base}]";
		$k++;
	    }

	    # l_context��r_context�������ǤȤ������������ڤ�Ƥ��뤫�����å�
	    $begin_check = 0;
	    $end_check = 0;
	    $length = 0;
	    for ($k = 0; $mrph[$i][$j][$k]{word}; $k++) {
		if ($length == length($l_context)) {
		    $begin_check = 1;
		    $mrph_start_num = $k;
		} 
		$length += $mrph[$i][$j][$k]{length};
		if ($length == (length($l_context) + 
				   length($data[$i][$j]{phrase}))) {
		    $end_check = 1;
		}
	    }
	    if (!$begin_check || !$end_check) {
		print STDERR "CONTEXT ERROR ($pattern)\n";
		return;
	    }

	    # part �� mrph ���б��Ĥ� 
	    # (���ˤ�ä�OR��ʸ�������㤦�Τǳ�$data[$i][$j]��ɬ��)

	    $part_length = 0;
	    $mrph_length = 0;
	    $k = $mrph_start_num;
	    for ($l = 0; $l < $part_num; $l++) {
		$part_length += $data[$i][$j]{length}[$l];
		$data[$i][$j]{start}[$l] = $k;
		for (; $mrph_length < $part_length; $k++) {
		    $mrph_length += $mrph[$i][$j][$k]{length};
		}
		$data[$i][$j]{end}[$l] = $k - 1;
		# print "($data[$i][$j]{start}[$l] $data[$i][$j]{end}[$l])";
	    }
	}
    }

    # OR����ʬ�Υޡ���

    $start_pos = 0;
    $error_flag = 0;
    for ($i = 0; $i < $part_num; $i++) {
	if ($feature[$i]{or}) {
	    for ($j = 1; $j < @{$part[$i]}; $j++) {

		# OR������η����ǿ��ΰ��ס� OR���������
		if ($data[0][0]{start}[$i] != $data[$i][$j]{start}[$i] ||
		    $data[0][0]{end}[$i] != $data[$i][$j]{end}[$i] ||
		    $data[0][0]{start}[$i] != $data[0][0]{end}[$i] ||
		    $data[$i][$j]{start}[$i] != $data[$i][$j]{end}[$i] ||
		    @{$mrph[0][0]} != @{$mrph[$i][$j]}) {
		    $error_flag = 1;
		}
		# OR�����η�������ΰ���
		for ($k = 0; $k < $data[0][0]{start}[$i]; $k++) {
		    if ($mrph[0][0][$k]{result} ne $mrph[$i][$j][$k]{result}) {
			$error_flag = 1;
			last;
		    }
		}
		# OR�θ�η�������ΰ���
		for ($k = $data[0][0]{end}[$i]+1; $k < @{$mrph[0][0]}; $k++) {
		    if ($mrph[0][0][$k]{result} ne $mrph[$i][$j][$k]{result}) {
			$error_flag = 1;
			last;
		    }
		}

		# OR��Ʊ���ʻ줫
		if ($mrph[0][0][$data[0][0]{start}[$i]]{pos} ne $mrph[$i][$j][$data[$i][$j]{start}[$i]]{pos}) {
		    $error_flag = 1;
		    if ($mrph[0][0][$data[0][0]{start}[$i]]{conj} eq "*" && 
			$mrph[$i][$j][$data[$i][$j]{start}[$i]]{conj} ne "*") {

			# $data[0][0]��ɸ��Ȥ���Τǡ�$data[0][0]��̵���ѡ�$data[i][j]�����Ѥξ��
			# $data[i][j]��word��base�˥��ԡ�����

			$mrph[$i][$j][$data[$i][$j]{start}[$i]]{base} = $mrph[$i][$j][$data[$i][$j]{start}[$i]]{word}
		    }
		}

		$WORD = $mrph[0][0][$data[0][0]{start}[$i]]{base} if ($j == 1);
		$WORD .= " $mrph[$i][$j][$data[$i][$j]{start}[$i]]{base}";
	    }
	    # print "$WORD\n";
	    $mrph[0][0][$data[0][0]{start}[$i]]{base} = "($WORD)";
	}
	$start_pos += $data[0][0]{length}[$i];
    }

    # OR�ξ����������ʤ���Х��顼����
    if ($error_flag) {
	for ($i = 0; $i < $part_num; $i++) {
	    for ($j = 0; $j < @{$part[$i]}; $j++) {
		next unless ($data[$i][$j]);
		print STDERR "ERROR($i,$j) ";
		for ($k = 0; $k < @{$mrph[$i][$j]}; $k++) {
		    print STDERR "$mrph[$i][$j][$k]{result}";
		}
		print STDERR "\n";
	    }
	}
	print STDERR "\n";
    }

    # ����

    print " ?*" if ($any_prefix && $part[0][0] ne "��");

    for ($i = 0; $i < $part_num; $i++) {
	if ($feature[$i]{result}) {
	    print $feature[$i]{result};
	    print "*" if ($feature[$i]{ast});
	}
	else {
	    for ($k = $data[0][0]{start}[$i]; $k <= $data[0][0]{end}[$i]; $k++) {
		if ($k == $data[0][0]{end}[$i] &&
		    $feature[$i]{lastGENERAL}) {
		    if ($mrph[0][0][$k]{conj} ne "*") {
			$mrph[0][0][$k]{result} = " [* * * * * ((���Ѹ�))]";
		    } else {
			$mrph[0][0][$k]{result} = " [* * * * * ((^���Ѹ�))]";
		    }
		}
		elsif (($k == $data[0][0]{end}[$i] &&
			$feature[$i]{lastgeneral}) ||
		       $g_define_word{$mrph[0][0][$k]{base}}) {
		    if ($mrph[0][0][$k]{conj} ne "*") {
			$conj_flag = 0;
			for ($m = 0; $m < @conj_type; $m++) {
			    if ($conj_type[$m] =~ /$mrph[0][0][$k]{conj}/) {
				$mrph[0][0][$k]{result} = " [* * ($conj_type[$m]) $mrph[0][0][$k]{conj2} *]";
				$conj_flag = 1;
				last;
			    }
			}
			if (!$conj_flag) {
			    print STDERR "Invalid conjugation type ($mrph[0][0][$k]{conj})!!\n";
			}
		    } else {
			$mrph[0][0][$k]{result} = " [* * * * * ((̾��������))]";
		    }
		}
		elsif ($k == $data[0][0]{end}[$i] &&
		       $feature[$i]{lastnum}) {
		    if ($feature[$i]{lastnum} =~ /1/) {
			$mrph[0][0][$k]{result} = " [$mrph[0][0][$k]{pos}";
		    } else {
			$mrph[0][0][$k]{result} = " [*";
		    }
		    if ($feature[$i]{lastnum} =~ /2/) {
			$mrph[0][0][$k]{result} .= " $mrph[0][0][$k]{pos2}";
		    } else {
			$mrph[0][0][$k]{result} .= " *";
		    }
		    if ($feature[$i]{lastnum} =~ /3/) {
			$mrph[0][0][$k]{result} .= " $mrph[0][0][$k]{conj}";
		    } else {
			$mrph[0][0][$k]{result} .= " *";
		    }
		    if ($feature[$i]{lastnum} =~ /4/) {
			$mrph[0][0][$k]{result} .= " $mrph[0][0][$k]{conj2}";
		    } else {
			$mrph[0][0][$k]{result} .= " *";
		    }
		    if ($feature[$i]{lastnum} =~ /5/) {
			$mrph[0][0][$k]{result} .= " $mrph[0][0][$k]{base}]";
		    } else {
			$mrph[0][0][$k]{result} .= " *]";
		    }
		}
		elsif ($mrph[0][0][$k]{base} eq "��") {
		    $mrph[0][0][$k]{result} = " [�ü� ���� * * *]";
		}
		elsif ($mrph[0][0][$k]{base} eq "��") {
		    $mrph[0][0][$k]{result} = " [�ü� ���� * * *]";
		}
		elsif ($mrph[0][0][$k]{base} eq "��") {
		    $mrph[0][0][$k]{result} = " [�ü� ��̻� * * *]";
		}
		elsif ($mrph[0][0][$k]{base} eq "��") {
		    $mrph[0][0][$k]{result} = " [�ü� ��̽� * * *]";
		}
		elsif ($mrph[0][0][$k]{base} eq "��") {
		    $mrph[0][0][$k]{result} = "";
		}
		else {
		    $mrph[0][0][$k]{pos2} = "*" if ($mrph[0][0][$k]{pos} eq "̾��");
		    $mrph[0][0][$k]{pos2} = "*" if ($mrph[0][0][$k]{pos} eq "����");
		    $mrph[0][0][$k]{pos2} = "*" if ($mrph[0][0][$k]{pos} eq "����");
		    $mrph[0][0][$k]{result} =
			" [$mrph[0][0][$k]{pos} $mrph[0][0][$k]{pos2} $mrph[0][0][$k]{conj} $mrph[0][0][$k]{conj2} $mrph[0][0][$k]{base}]";
		}

		# �����Ǥ�feature�����ꤵ��Ƥ�����
 		if ($k == $data[0][0]{end}[$i] &&
		    $feature[$i]{lastfeature}) {
		    if ($mrph[0][0][$k]{result}) {
			$mrph[0][0][$k]{result} =~ s/\]$/$feature[$i]{lastfeature}\]/;
		    }
		    else {
			$mrph[0][0][$k]{result} = " [* * * * *$feature[$i]{lastfeature}]";
		    }
		}

		print $mrph[0][0][$k]{result};
		print "*" if ($feature[$i]{ast});
	    }

	    if ($feature[$i]{ast} &&
		$data[0][0]{end}[$i] - $data[0][0]{start}[$i] > 0) {
		print STDERR "$input: *�ϳƷ����ǤˤĤ�����������\n";
	    }
	}
    }
}

######################################################################
