/*
 * Generate a HTTP GET query string including the leading ?.  params is a map
 * of (parameter name -> value).
 */
exports.queryString = function(params) {
    var varList = [];
    for(p in params)
        varList.push(p + '=' + encodeURI(params[p]));

    return '?' + varList.join('&');
}

exports.mapValues = function(obj, f) {
    var result = {};
    for(key in obj) result[key] = f(obj[key]);
    return result;
}

exports.values = function(obj) {
    var result = [];
    for(key in obj) result.push(obj[key]);
    return result;
}

