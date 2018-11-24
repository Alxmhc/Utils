//a.x*r1+a.y*r2+a.z=0; b.x*r1+b.y*r2+b.z=0
function eq1_2(a,b){
	let q = vt3.pr_v3(a,b);
	return new vt2(q.x/q.z, q.y/q.z);
}
