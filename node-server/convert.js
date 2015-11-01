function convertDataToString(data){
  function fullCoordinates(coord){
	var result = "";
	for(var i in coord) {
		result += coord[i] + " ";
	}
    return result + "\n";
  }

  function getReciever(r){
    return r[0] + "\t" + r[1] + "\t" + r[2] + "\t" + "\n";
  }
	
	var ret;
	
	var incn = data.includes.length;
	
	ret = "" + fullCoordinates(data.area) + "\n" + incn + "\n\n";
	
	
	for(var i = 0; i < incn; i++) {
		ret = ret + fullCoordinates(data.includes[i]) + "\n";
	}
	
	var N = data.receivers.length;
	
	ret = ret + N + "\n";
	
	for(var i = 0; i < N; i++) {
		ret = ret + getReciever(data.receivers[i]);
	}
	ret = ret + "\n";
	
	ret = ret + ((data.config.alpha.length > 0) ? "1" : "0") + "\n" + 
				((data.config.gamma && data.config.gamma.length > 0) ? "1" : "0") + "\n"
	
	if(data.config.alpha.length > 0) {
		ret = ret + data.config.alpha[0] + "\n" +
				data.config.alpha[1] + "\n" +
				data.config.alpha[2] + "\n";
	}
	if(typeof(data.config.gamma) != "undefined" && data.config.gamma.length > 0) {
		ret = ret + data.config.gamma[0] + "\n" +
				data.config.gamma[1] + "\n" +
				data.config.gamma[2] + "\n" +
				data.config.gamma[3] + "\n";
	}
	return ret;		
}

exports.convertDataToString = convertDataToString;

