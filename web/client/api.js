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
            token: localStorage.getItem('token')
        }
        );

    console.log('api request: ' + requestContent);

    req.open('post', '/api_call', true);
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

var passthroughFunctions = {
    'features':                  'features',

    'addPhotographToAlbum':      'add_photograph_to_album',
    'albumList':                 'album_list',
    'deleteAlbum':               'delete_album',
    'insertAlbum':               'insert_album',
    'updateAlbum':               'update_album',

    'photograph':                'photograph',
    'deletePhotograph':          'delete_photograph',
    'insertPhotograph':          'insert_photograph',
    'photographAlbums':          'photograph_albums',
    'photographList':            'photograph_list',
    'photographsInAlbum':        'photographs_in_album',
    'photographsWithLocation':   'photographs_with_location',
    'photographsWithTag':        'photographs_with_tag',
    'recentPhotographs':         'recent_photographs',
    'removePhotographFromAlbum': 'remove_photograph_from_album',
    'uncategorisedPhotographs':  'uncategorised_photographs',
    'updatePhotograph':          'update_photograph',

    'markdownData':              'markdown_data',
    'note':                      'note',
    'noteList':                  'note_list',
    'noteVersion':               'note_version',
    'notePhaseVersion':          'note_phase_version',
    'insertNote':                'insert_note',
    'createDraftNote':           'create_draft_note',
    'deleteNote':                'delete_note',
    'updateMarkdownData':        'update_markdown_data',
    'publishNote':               'publish_note',
    'publishedNote':             'published_note',
    'publishedNotes':            'published_notes',

    'locations':                 'locations',
    'tags':                      'tags',

    'statistics':                'statistics',

    'getTile':                   'get_tile',
    'searchGazetteer':           'search_gazetteer',

    'login':                     'login',
    'logout':                    'logout',
    'tokenValid':                'token_valid',
    'updateUser':                'update_user',
    'loggedInUser':              'logged_in_user'
};

for(f in passthroughFunctions)
    exports[f] = exports.passThrough.bind(this, passthroughFunctions[f]);

