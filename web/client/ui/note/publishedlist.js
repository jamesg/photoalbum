var _     = require('underscore');
var domjs = require('domjs/lib/html5')(document);

var api = require('../../api');

/*
 * Callback is called with the id of the note selected.
 */
exports.PublishedNoteList = function(callback) {
    var reload = function() {
        api.publishedNotes(
                (function(err, notes) {
                    if(err)
                        console.log('fetching published notes: ' + e);
                    else
                        _setNoteList(notes);
                }).bind(this)
                );
    }

    var _setNoteList = function(notes) {
        _element().innerHTML = '';
        _element(domjs.build(_.partial(_listTemplate, notes)));
    }

    var _listTemplate = function(notes) {
        return div(
            { class: 'pure-menu pure-menu-open' },
            span({ class: 'pure-menu-heading' }, 'Notes'),
            ul.apply(
                this,
                _.map(
                    notes,
                    function(note) {
                        return li(
                            a(
                                { onclick: _.partial(_callback, note.note_id) },
                                note.created, ' ', note.title
                                )
                            );
                    })
                )
            );
    }

    var _template = function() {
        _element = div();
        _element();
    }

    var _callback = callback?callback:(function() { });
    var _documentFragment = domjs.build(_template);
    var _element;

    reload();

    return {
        element: function() { return _element; }
    };
}

