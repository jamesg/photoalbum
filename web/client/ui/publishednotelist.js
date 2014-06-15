var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

/*
 * Callback is called with the id of the note selected.
 */
exports.PublishedNoteList = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );

    this.reload();
}

exports.PublishedNoteList.prototype.reload = function() {
    api.publishedNotes(
            (function(err, notes) {
                if(err)
                    console.log('fetching published notes: ' + e);
                else
                    this._setNoteList(notes);
            }).bind(this)
            );
}

exports.PublishedNoteList.prototype._setNoteList = function(notes) {
    this._element().innerHTML = '';
    this._element(
            { class: 'pure-table' },
            domjs.build(
                this._listTemplate.bind(this, notes)
                )
            );
}

exports.PublishedNoteList.prototype.element = function() {
    return this._element;
}

exports.PublishedNoteList.prototype._listTemplate = function(notes) {
    this._element(thead(tr(td('Date'), td('Title'), td('Action'))));
    var _tbody = tbody();
    this._element(_tbody);
    for(n in notes) {
        var _tr = tr(
                td(notes[n].created),
                td(notes[n].title)
                );
        if(this._callback)
            _tr(
                td(
                    button(
                        {
                            class: 'pure-button',
                            onclick: this._callback.bind(this, notes[n].note_id)
                        },
                        'View'
                        )
                  )
                );
        _tbody(_tr);
    }
}

exports.PublishedNoteList.prototype._template = function() {
    this._element = table({ 'class': 'notetable' });
    this._element();
}

