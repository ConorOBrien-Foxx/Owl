# for verifying programs

$VOWEL = /[aeiouy]/
$REG = /[aei]|o#$VOWEL|u#$VOWEL{2}|y#$VOWEL{3}/

$indent = 0
puts STDIN.read
    .gsub(/\|.+$/, "")
    .scan(/#$VOWEL/)
    .join
    .scan(/(a)(#$REG)(#$REG)|([ei])(#$REG)|(o.)(#$REG)(#$REG)(#$REG)|(u)|(.)/)
    .map(&:compact)
    .map { |cmd, *ops|
        $indent -= 1 if cmd == "u"
        res = " " * 2 * $indent + cmd.ljust(4) + ops.join(", ")
        $indent += 1 if cmd == "i"
        res
    }