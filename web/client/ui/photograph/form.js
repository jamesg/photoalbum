var domjs = require('domjs/lib/html5')(document);

var util  = require('../../util');

var MessageBox = require('../messagebox').MessageBox;

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
        if(submissionData.title == '')
        {
            messageBox.displayWarning('Title is required');
            return false;
        }
        if(submissionData.location == '')
        {
            messageBox.displayWarning('Location is required');
            return false;
        }

        var photographData = new FormData(_form());

        _form.disabled = true;
        messageBox.displayInfo('Uploading');
        var reqListener = function() {
            _form.disabled = false;
            if(this.response['error'])
                messageBox.displayError(this.response.error);
            else
                messageBox.displaySuccess('Upload Complete');
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
                        console.log('upload percentage', percent);
                    }
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

    var messageBox = new MessageBox();

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
        messageBox.element(),
        inlineInput(inputs.title, 'Title'),
        inlineInput(inputs.caption, 'Caption'),
        inlineInput(inputs.location, 'Location'),
        inlineInput(inputs.tags, 'Tags'),
        inlineInput(fileInput, 'File'),
        inlineInput(submitButton, '')
        );
    this._element = div(
            { class: 'content-view pure-g' },
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

