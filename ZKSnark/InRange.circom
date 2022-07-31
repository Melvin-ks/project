
template Num2Bits(n) {
    signal input in;
    signal output out[n];
    var lc1=0;

    var e2=1;
    for (var i = 0; i<n; i++) {
        out[i] <-- (in >> i) & 1;
        out[i] * (out[i] -1 ) === 0;
        lc1 += out[i] * e2;
        e2 = e2+e2;
    }

    lc1 === in;
}


template Less(n) {
    assert(n <= 252);
    signal input in[2];
    signal output out;

    component n2b = Num2Bits(n+1);

    n2b.in <== in[0]+ (1<<n) - in[1];

    out <== 1-n2b.out[n];
}

template LessEq(n) {
    signal input in[2];
    signal output out;

    component lt = Less(n);

    lt.in[0] <== in[0];
    lt.in[1] <== in[1]+1;
    lt.out ==> out;
}

template Greater(n) {
    signal input in[2];
    signal output out;

    component lt = Less(n);

    lt.in[0] <== in[1];
    lt.in[1] <== in[0];
    lt.out ==> out;
}


template GreaterEq(n) {
    signal input in[2];
    signal output out;

    component lt = Less(n);

    lt.in[0] <== in[1];
    lt.in[1] <== in[0]+1;
    lt.out ==> out;
}

template InRange(){

    
    signal input range[2]; 
    
    
    signal  input CET6[1];
    signal output out; 

    component gt1 = GreaterEq(6);
    
    gt1.in[0] <== CET6[0];
    gt1.in[1] <== range[0];
    gt1.out === 1; 

   
    component lt1 = LessEq(6);
    lt1.in[0] <== CET6[0];
    lt1.in[1] <== range[1];
    lt1.out === 1;
   

    out <-- (gt1.out + lt1.out);
    out === 2;
}


component main = InRange();


