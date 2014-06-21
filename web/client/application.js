var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var css = require('./css').css;

var Albums         = require('./albums').Albums;
var ChangePassword = require('./changepassword').ChangePassword;
var Filters        = require('./filters').Filters;
var Locations      = require('./locations').Locations;
var LoginForm      = require('./loginform').LoginForm;
var MainMenu       = require('./ui/mainmenu').MainMenu;
var Map            = require('./map').Map;
var NewPhotograph  = require('./newphotograph').NewPhotograph;
var NoteEditor     = require('./noteeditor').NoteEditor;
var Notes          = require('./notes').Notes;
var Statistics     = require('./statistics').Statistics;
var Tags           = require('./tags').Tags;

exports.Application = function() {
    this._documentFragment = domjs.build(this._template.bind(this));
    api.tokenValid(
            localStorage.getItem('token'),
            (function(err, result) {
                if(result) 
                    this.showMainMenu();
                else
                    this.showLoginForm();
            }).bind(this)
            );
}

var _editButton;

exports.Application.prototype._template = function() {
    this._element = div();

    _editButton = button(
            { onclick: this._toggleEdit.bind(this) },
            'Hide Controls'
            );

    this._element(
            h1({ 'onclick': this.showMainMenu.bind(this) }, 'Photograph Album')
            );

    this._container = div();
    this._element(this._container);
}

exports.Application.prototype.element = function() {
    return this._element;
}

var _showEdit = true;

exports.Application.prototype._toggleEdit = function() {
    var rule = css.getOrCreateCssRule('.editable');

    if(_showEdit)
    {
        rule.style.display = 'none';
        _editButton().innerHTML = 'Show Controls';
        _showEdit = false;
    }
    else
    {
        rule.style.display = 'block';
        _editButton().innerHTML = 'Hide Controls';
        _showEdit = true;
    }
}

exports.Application.prototype._showElement = function(element) {
    this._container().innerHTML = '';
    this._container(element);
}

exports.Application.prototype.showMainMenu = function() {
    this._showElement((new MainMenu(this)).element());
}

exports.Application.prototype.showAlbums = function() {
    this._showElement((new Albums()).element());
}

exports.Application.prototype.showLocations = function() {
    this._showElement((new Locations()).element());
}

exports.Application.prototype.showLoginForm = function() {
    this._showElement((new LoginForm(this.showMainMenu.bind(this))).element());
}

exports.Application.prototype.showTags = function() {
    this._showElement((new Tags()).element());
}

exports.Application.prototype.showFilters = function() {
    this._showElement((new Filters()).element());
}

exports.Application.prototype.showNoteEditor = function() {
    this._showElement((new NoteEditor()).element());
}

exports.Application.prototype.showNotes = function() {
    this._showElement((new Notes()).element());
}

exports.Application.prototype.showNewPhotograph = function() {
    this._showElement((new NewPhotograph()).element());
}

exports.Application.prototype.showStatistics = function() {
    this._showElement((new Statistics()).element());
}

exports.Application.prototype.showMap = function() {
    this._showElement((new Map()).element());
}

exports.Application.prototype.changePassword = function() {
    this._showElement((new ChangePassword()).element());
}

