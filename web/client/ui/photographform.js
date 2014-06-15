var domjs = require('domjs/lib/html5')(document);

var util  = require('../util');

/*
 * New photograph form.
 */
exports.PhotographForm = function() {
    domjs.build(this._template.bind(this));
}

exports.PhotographForm.prototype.element = function() {
    return this._element();
}

/*
 * Template for the photograph form.
 */
exports.PhotographForm.prototype._template = function() {
    var submit = function() {
        var submissionData = util.mapValues(
            inputs,
            function(input) { return input().value; }
            );
        messageBox().innerHTML = 'Form submit';
        if(submissionData.title == '')
        {
            messageBox().innerHTML = 'Title is required';
            return false;
        }
        if(submissionData.location == '')
        {
            messageBox().innerHTML = 'Location is required';
            return false;
        }

        var photographData = new FormData(_form());

        _form.disabled = true;
        messageBox().innerHTML = 'Uploading';
        var reqListener = function() {
            _form.disabled = false;
            if(this.response['error'])
                console.log('upload error ' + this.response.error);
            else
                console.log('upload success');
        }

        var xhr = new XMLHttpRequest();
        xhr.open(
                'post',
                '/insert_photograph',
                true
                );
        xhr.onload = reqListener;
        xhr.addEventListener(
                'progress',
                function(event) {
                    if(event.lengthComputable)
                    {
                        var percent = (event.loaded/event.total)*100;
                        messageBox().innerHTML =
                            'Progress: ' + percent + '%';
                    }
                    else
                        messageBox().innerHTML = 'Progress';
                }
                );
        xhr.addEventListener(
                'load',
                function(event) {
                    if(this.response['error'])
                        messageBox().innerHTML = 'Upload error';
                    else
                        messageBox().innerHTML = 'Upload complete';
                }
                );
        xhr.send(photographData);

        return false;
    }

    var inputs = {
        title:    input({ type: 'text', name: 'title' }),
        caption:  input({ type: 'text', name: 'caption' }),
        location: input({ type: 'text', name: 'location' }),
        tags:     input({ type: 'text', name: 'tags', placeholder: 'Comma-separated tags' })
    };
    var submitButton = input(
        {
            class: 'pure-button pure-button-primary',
            type: 'submit',
            value: 'Upload'
        }
        );
    var fileInput = input({ type: 'file', name: 'file' });

    var messageBox = span({ class: 'messagebox' });

    var inlineInput = function(input_, label_) {
        return div(
                { class: 'pure-control-group' },
                label({ for: input_().name }, label_),
                input_
                );
    }

    var _form = form(
        {
            class: 'pure-form pure-form-aligned',
            method: 'POST',
            enctype: 'multipart/form-data',
            onsubmit: submit
        },
        messageBox,
        inlineInput(inputs.title, 'Title'),
        inlineInput(inputs.caption, 'Caption'),
        inlineInput(inputs.location, 'Location'),
        inlineInput(inputs.tags, 'Tags'),
        inlineInput(fileInput, 'File'),
        submitButton
        );
    this._element = div(
            { class: 'mainmenu pure-g' },
            div(
                { class: 'pure-u-1-1' },
                h2(
                    'New Photograph ',
                    small('Upload a photograph from your computer')
                    ),
                _form
               )
            );
}

