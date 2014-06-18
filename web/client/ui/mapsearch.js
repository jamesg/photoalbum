var _ = require('underscore');
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

    this._results = div(
        { class: 'pure-u-3-5' },
        this._listTemplate([])
        );
    this._element(
            { class: 'pure-g' },
            div(
                { class: 'pure-u-2-5' },
                h2('Search ', small('Find a feature by name.')),
                this._formTemplate()
                ),
            this._results
            );
};

exports.MapSearch.prototype._formTemplate = function() {
    var searchTerm = input({ type: 'text' });
    return form(
        {
            class: 'pure-form',
            onsubmit: (function() {
                if(searchTerm().value != '')
                    this._search(searchTerm().value);
                return false;
            }).bind(this)
        },
        label('Search term', searchTerm),
        input({ class: 'pure-button', type: 'submit', value: 'Search' })
        );
};

exports.MapSearch.prototype._listTemplate = function(results) {
    return table(
        { class: 'pure-table' },
        thead(tr(th('Name'), th('KM Ref'), th())),
        tbody(
            _.map(
                results,
                (function(result) {
                    return tr(
                        td(result.def_nam),
                        td(result.km_ref),
                        td(
                            button(
                                {
                                    class: 'pure-button',
                                    onclick: this._callback.bind(this, result.km_ref)
                                },
                                'Tile'
                                )
                            )
                        );
                }).bind(this)
                )
            )
        );
};

exports.MapSearch.prototype._search = function(searchTerm) {
    api.searchGazetteer(
            searchTerm,
            (function(err, result) {
                console.log(result);
                this._results().innerHTML = '';
                if(err) {
                    console.log(error);
                }
                this._results(domjs.build(this._listTemplate.bind(this, result)));
            }).bind(this)
            );
};

