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
  <br>Note that Unicode characters may require multiple `?` input.

Example usage:

```
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
A total of 4 matches found.
tak
tek
tik
tok

Search: t??k
A total of 5 matches found.
talk
tank
terk
tık
trok
```
