var _ = require('underscore');

var util = require('./util');

/*
 * Make an API request.  Last argument is a callback function(err, result).
 */
exports.apiRequest = function(api_function, params, callback) {
    var req = new XMLHttpRequest();
    var reqListener = function() {
        if(!callback) return;
        if(this.response['error'])
            callback(this.response.error, null);
        else
            callback(null, JSON.parse(this.responseText).result);
    }

    req.open(
            'get',
            api_function + util.queryString(params),
            true
            );
    req.onload = reqListener;
    req.send();
}

exports.passThrough = function() {
    var api_function = arguments[0];
    var params = [];
    for(var i = 1; i < arguments.length-1; ++i)
        params.push(arguments[i]);
    var callback = arguments[arguments.length-1];

    var req = new XMLHttpRequest();
    var reqListener = function() {
        console.log('api response: ' + this.responseText);
        var jsonIn = JSON.parse(this.responseText);
        if(callback) {
            if(jsonIn['result'])
                callback(null, jsonIn['result']);
            else
                callback(jsonIn['error']);
        }
    }

    var requestContent = JSON.stringify(
        {
            'method': api_function,
            'params': params,
            token: window.localStorage.getItem('token')
        }
        );

    console.log('api request: ' + requestContent);

    req.open('post', '/api_call', true);
    req.setRequestHeader('Authorization', window.localStorage.getItem('token'));
    req.onload = reqListener;
    req.send(requestContent);
}

exports.apiPostRequest = function(api_function, params, callback) {
    var req = new XMLHttpRequest();
    var reqListener = function() {
        if(!callback) return;
        if(this.response['error'])
            callback(this.response.error, null);
        else
            callback(null, this.response.result);
    }

    var formData = FormData();
    for(key in params)
        formData.append(key, params[key]);

    req.open(
            'post',
            api_function,
            true
            );
    req.responseType = 'json';
    req.onload = reqListener;
    req.send(formData);
}

/*
 * List of functions that will be install on exports.api.
 */
var passthroughFunctions = [
    'features',

    'addPhotographToAlbum',
    'albumList',
    'deleteAlbum',
    'insertAlbum',
    'updateAlbum',

    'photograph',
    'deletePhotograph',
    'insertPhotograph',
    'photographAlbums',
    'photographList',
    'photographsInAlbum',
    'photographsWithLocation',
    'photographsWithTag',
    'recentPhotographs',
    'removePhotographFromAlbum',
    'uncategorisedPhotographs',
    'updatePhotograph',

    'markdownData',
    'note',
    'noteList',
    'noteVersion',
    'notePhaseVersion',
    'insertNote',
    'createDraftNote',
    'deleteNote',
    'updateMarkdownData',
    'publishNote',
    'publishedNote',
    'publishedNotes',

    'locations',
    'tags',
    'tagsAlphabetical',
    'tagsPopular',

    'statistics',

    'getTile',
    'searchGazetteer',

    'login',
    'logout',
    'tokenValid',
    'updateUser',
    'loggedInUser',
];

// Install passthrough functions as members of 'exports'.
_.each(
        passthroughFunctions,
        function(f) {
            exports[f] = exports.passThrough.bind(
                this,
                // Translate the local function name (lower camel case) to an
                // underscore-separated name.
                _.flatten(
                    _.map(f, function(c) {
                        return (/[A-Z]/.test(c))?('_' + c.toLowerCase()):c;
                    })
                    ).join('')
                );
        }
        );

