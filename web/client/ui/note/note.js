var domjs  = require('domjs/lib/html5')(document);
var marked = require('marked/lib/marked');

var api = require('../../api');

exports.Note = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Note.prototype.setNote = function(noteId, phase) {
    this._noteId = noteId;
    this._phase = phase;

    api.markdownData(
        noteId,
        phase,
        (function(err, markdownData) {
            console.log(markdownData);
            if(err) {
                console.log(err);
                return;
            }
            this._markdownId = markdownData.markdown_id;
            api.note(
                noteId,
                (function(err, note) {
                    this.displayMarkdown(note.title, markdownData);
                }).bind(this)
                );
        }).bind(this)
        );
}

exports.Note.prototype.displayMarkdown = function(title, markdownData) {
    this._markdownId = markdownData.markdown_id;
    this._element().innerHTML = marked(markdownData.markdown);
}

exports.Note.prototype.element = function() {
    return this._element;
}

exports.Note.prototype._template = function() {
    this._element = div();
    this._element();

    this._element(this._loadingTemplate());
}

exports.Note.prototype._loadingTemplate = function() {
    p('Loading');
}

