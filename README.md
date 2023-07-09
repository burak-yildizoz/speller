# speller

## speller_regex

Greedy search given regex string in spelling database.

Example usage:

```
Search: asd
A total of 7 matches found.
dasdaracık
tasdi
tasdik
tasdik etmek
tasdikli
tasdikname
tasdiksiz

Search: ^[piyano][telefon][anahtar][sandalye][kedi][bavul][fare][yılan]$
A total of 2 matches found.
pehlivan
porselen
```

## speller_search

Find exact match given search string.

The following characters are specially treated.

- `*`

  Match one or more characters.

- `?`

  Match exactly one character.
  <br>Note that an alphabet file must be provided in order for Unicode characters to be treated correctly.

Example usage:

```
Speller filename: res/tr.txt
Alphabet filename: res/alfabe.txt
Locale: tr

Search: *asd*
A total of 7 matches found.
dasdaracık
tasdi
tasdik
tasdik etmek
tasdikli
tasdikname
tasdiksiz

Search: t?k
A total of 5 matches found.
tak
tek
tık
tik
tok
```

## speller_search_any

Find words that contain given letter set.

The following character is specially treated.

- `?`

  Wildcard for one character.
  <br>Note that an alphabet file must be provided in order for Unicode characters to be treated correctly.

```
Speller filename: res/tr.txt
Alphabet filename: res/alfabe.txt
Locale: tr

Search: eiityz???
A total of 8 matches found.
eziyetsiz       esz
meziyetli       eml
riayetsiz       asr
seyitgazi       gas
zeytinlik       lnk
zeytinsiz       nsz
zilyetlik       llk
ziyaretçi       çar

Search: aaksuv???
A total of 3 matches found.
savrulmak       mlr
savurtmak       mtr
uçaksavar       çar
```
