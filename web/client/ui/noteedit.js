var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

exports.NoteEdit = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.NoteEdit.prototype.setNote = function(noteId, phase) {
    this._noteId = noteId;
    this._phase = phase;

    api.markdownData(
        noteId,
        phase,
        (function(err, markdownData) {
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
                )
        }).bind(this)
        );
}

exports.NoteEdit.prototype.displayMarkdown = function(title, markdownData) {
    console.log('display markdown: ' + markdownData.markdown);
    this._markdownId = markdownData.markdown_id;
    this._element().innerHTML = '';
    this._element(
            domjs.build(
                this._editorTemplate.bind(this, title, markdownData.markdown)
                )
            );
}

exports.NoteEdit.prototype.element = function() {
    return this._element;
}

exports.NoteEdit.prototype._template = function() {
    this._element = div();

    this._element(this._loadingTemplate());
}

exports.NoteEdit.prototype._loadingTemplate = function() {
    p('Loading');
}

exports.NoteEdit.prototype._editorTemplate = function(title, markdown) {
    var markdownTextarea = textarea(
            {
                'name': 'markdown',
                'rows': 16,
                'cols': 80
            },
            markdown
            );
    var statusP = p();
    form(
        {
            'onsubmit': (function() {
                api.updateMarkdownData(
                    {
                        'markdown_id': this._markdownId,
                        'markdown': markdownTextarea().value
                    },
                    function(err) {
                        if(err)
                            console.log('update markdown: ' + err);
                        else
                            statusP().innerHTML = 'Note updated.';
                    }
                    );
                return false;
            }).bind(this)
        },
        p('Title: ' + title),
        markdownTextarea,
        br(),
        input(
            {
                class: 'pure-button pure-button-primary',
                type: 'submit',
                value: 'Save'
            }
            )
        );
}

