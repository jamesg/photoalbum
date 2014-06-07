var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

exports.MapSearch = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(this._template.bind(this));
}

exports.MapSearch.prototype.element = function() {
    return this._element;
}

exports.MapSearch.prototype._template = function() {
    this._element = div();

    var searchTerm = input({ 'type': 'text' });
    this._results = table();
    this._listTemplate([]);
    this._element(
            form(
                {
                    'onsubmit': (function() {
                        if(searchTerm().value != '')
                            this._search(searchTerm().value);
                        return false;
                    }).bind(this)
                },
                searchTerm,
                input({ 'type': 'submit', 'value': 'Search' })
                ),
            this._results
            );
}

exports.MapSearch.prototype._listTemplate = function(result) {
    this._results(tr(td('Name'), td('KM Ref'), td()));
    for(i in result)
    {
        this._results(
                tr(
                    td(result[i].def_nam),
                    td(result[i].km_ref),
                    td(
                        span(
                            {
                                'onclick': this._callback.bind(this, result[i].km_ref)
                            },
                            'Tile'
                            )
                        )
                    )
                );
    }
}

exports.MapSearch.prototype._search = function(searchTerm) {
    api.searchGazetteer(
            searchTerm,
            (function(err, result) {
                console.log(result);
                this._results().innerHTML = '';
                if(err) {
                    console.log(error);
                }
                domjs.build(this._listTemplate.bind(this, result));
            }).bind(this)
            );
}

