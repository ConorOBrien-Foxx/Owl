$vowels = "aeiouy"
def vowel_to_int(s)
    return Float::NAN if s.empty?
    rest = s[1..3].tr($vowels, "012345").to_i 6
    case s[0]
    when "o"
        return Float::NAN if s[1].nil? || s[2]
        v = rest + 3
    when "u"
        return Float::NAN if s[2].nil? || s[3]
        v = rest + 9
    when "y"
        return Float::NAN if s[3].nil? || (s[4] && rest != 215)
        v = rest + 45
    else
        v = "aei".index(s)
    end
    if v == 260
        v += vowel_to_int(s[4..-1])
    end
    v || Float::NAN
end

def int_to_vowel(n)
    if n >= 260
        ycount = n / 260
        "yyyy" * ycount + int_to_vowel(n % 260)
    elsif n < 3
        $vowels[n]
    elsif n < 9
        n -= 3
        "o" + $vowels[n]
    elsif n < 45
        n -= 9
        "u" + $vowels[(n / $vowels.size) % $vowels.size] + $vowels[n % $vowels.size]
    else
        n -= 45
        "y" + $vowels[(n / $vowels.size / $vowels.size) % $vowels.size] + $vowels[(n / $vowels.size) % $vowels.size] + $vowels[n % $vowels.size]
    end
    
end

ARGV.each { |arg|
    if /\d/ === arg
        puts "#{arg} => #{int_to_vowel arg.to_i}"
    else
        puts "#{arg} => #{vowel_to_int arg}"
    end
}