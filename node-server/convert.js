function fullCordinates(coord){
	return coord.xmin + " " + coord.xmax + "\n" +
		coord.ymin + " " + coord.ymax + "\n" +
		coord.zmin + " " + coord.zmax + "\n" +
		coord.xnum + " " + coord.ynum + " " + coord.znum + "\n";
}

function getReciever(r){
	return r.x + "\t" + r.y + "\t" + r.z + "\t" + "\n";
}

function convertDataToString(data){
	
	var ret;
	ret = "" + fullCordinates(data.area) + "\n" + 
		data.area.incnum + "\n" + "\n";
	
	var incn = data.area.incnum;
	for(var i = 0; i < incn; i++) {
		ret = ret + fullCoordinates(data.area.includes[i]) + "\n";
	}
	
	ret = ret + data.receivers.N + "\n";
	
	var N = data.receivers.N;
	
	for(var i = 0; i < N; i++) {
		ret = ret + getReciever(data.recievers.rcvr[i]);
	}
	ret = ret + "\n";
	
	ret = ret + data.config.use_alpha + "\n" + 
				data.config.use_gamma + "\n" +
				data.config.alpha0 + "\n" +
				data.config.dalpha + "\n" +
				data.config.alpha_coeff + "\n" +
				data.config.gamma0 + "\n" +
				data.config.dgamma + "\n" +
				data.config.gamma_coeff + "\n" +
				data.config.gamma_diff + "\n";
				
	return ret;		
}

exports.convertDataToString = convertDataToString;

