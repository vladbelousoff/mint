// Edge case: Empty file (handled by existing test)

// Edge case: Only whitespace
    
    
    

// Edge case: Only comments
// Comment 1
// Comment 2

/* Multiline
   comment */

// Edge case: Keywords as part of identifiers
letx ifelse whiletrue struct_trait implfor

// Edge case: Adjacent operators and symbols
a+++b  c---d  e===f  g!==h  i<<j  k>>l

// Edge case: String literals
"Empty string"
"String with \"escaped quotes\""
"String with \nescaped newline"
"String with backslash \\"
"String with 
multiple
lines"

// Edge case: Number edge cases
0 0.0 .0 0. 1e6 1.5e-3

// Edge case: Identifiers with underscores and numbers
_identifier _123 a_b_c abc123 a1b2c3 