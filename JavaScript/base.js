//n>=0
function shl(a,n){return n<32?a<<n:0}
function shr(a,n){return n<32?a>>>n:0}
function rotl(a,n){n&=31;return n?(a<<n)|(a>>>(32-n)):a}
function rotr(a,n){n&=31;return n?(a>>>n)|(a<<(32-n)):a}
