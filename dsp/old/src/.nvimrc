version 6.0
nnoremap 	 z=
nnoremap 	  i
noremap  0r#
noremap  0r 
nnoremap  :w
map  c :Calendar
map   
map  g G
map  ll l
map  kk k
map  jj j
map  hh h
map  j s
map  l v
let s:cpo_save=&cpo
set cpo&vim
map    <S-:>
map  p <F10>
map  o <F9>
map  i <F8>
map  u <F7>
map  y <F6>
map  t <F5>
map  r <F4>
map  e <F3>
map  w <F2>
map  q <F1>
omap <silent> % <Plug>(MatchitOperationForward)
xmap <silent> % <Plug>(MatchitVisualForward)
nmap <silent> % <Plug>(MatchitNormalForward)
nnoremap ''' :q
nnoremap ;;' :w:bd
omap <silent> [% <Plug>(MatchitOperationMultiBackward)
xmap <silent> [% <Plug>(MatchitVisualMultiBackward)
nmap <silent> [% <Plug>(MatchitNormalMultiBackward)
omap <silent> ]% <Plug>(MatchitOperationMultiForward)
xmap <silent> ]% <Plug>(MatchitVisualMultiForward)
nmap <silent> ]% <Plug>(MatchitNormalMultiForward)
xmap a% <Plug>(MatchitVisualTextObject)
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
omap <silent> g% <Plug>(MatchitOperationBackward)
xmap <silent> g% <Plug>(MatchitVisualBackward)
nmap <silent> g% <Plug>(MatchitNormalBackward)
nnoremap mn [s
nnoremap nm ]s 
nnoremap nn n
nnoremap oo o
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
vmap <silent> <Plug>(MatchitVisualTextObject) <Plug>(MatchitVisualMultiBackward)o<Plug>(MatchitVisualMultiForward)
onoremap <silent> <Plug>(MatchitOperationMultiForward) :call matchit#MultiMatch("W",  "o")
onoremap <silent> <Plug>(MatchitOperationMultiBackward) :call matchit#MultiMatch("bW", "o")
vnoremap <silent> <Plug>(MatchitVisualMultiForward) :call matchit#MultiMatch("W",  "n")m'gv``
vnoremap <silent> <Plug>(MatchitVisualMultiBackward) :call matchit#MultiMatch("bW", "n")m'gv``
nnoremap <silent> <Plug>(MatchitNormalMultiForward) :call matchit#MultiMatch("W",  "n")
nnoremap <silent> <Plug>(MatchitNormalMultiBackward) :call matchit#MultiMatch("bW", "n")
onoremap <silent> <Plug>(MatchitOperationBackward) :call matchit#Match_wrapper('',0,'o')
onoremap <silent> <Plug>(MatchitOperationForward) :call matchit#Match_wrapper('',1,'o')
vnoremap <silent> <Plug>(MatchitVisualBackward) :call matchit#Match_wrapper('',0,'v')m'gv``
vnoremap <silent> <Plug>(MatchitVisualForward) :call matchit#Match_wrapper('',1,'v')m'gv``
nnoremap <silent> <Plug>(MatchitNormalBackward) :call matchit#Match_wrapper('',0,'n')
nnoremap <silent> <Plug>(MatchitNormalForward) :call matchit#Match_wrapper('',1,'n')
map <F4> :setlocal spell! :hi SpellBad cterm=underline 
map <F2> :e C:/Users/josep/Documents/jobis_vailt/ 
map <F1> :e C:/Users/josep/AppData/Local/nvim/init.vim 
nnoremap <BS> <C-6>
nnoremap <C-BS> db
inoremap <NL> 
inoremap  
inoremap  :w
inoremap jj  
let &cpo=s:cpo_save
unlet s:cpo_save
set expandtab
set helplang=en
set hidden
set ignorecase
set path=.,/usr/include,,,**
set runtimepath=~/.config/nvim,/etc/xdg/xdg-ubuntu/nvim,/etc/xdg/nvim,~/.local/share/nvim/site,/usr/share/ubuntu/nvim/site,/usr/local/share/nvim/site,/usr/share/nvim/site,/var/lib/snapd/desktop/nvim/site,/usr/share/nvim/runtime,/usr/share/nvim/runtime/pack/dist/opt/matchit,/var/lib/snapd/desktop/nvim/site/after,/usr/share/nvim/site/after,/usr/local/share/nvim/site/after,/usr/share/ubuntu/nvim/site/after,~/.local/share/nvim/site/after,/etc/xdg/nvim/after,/etc/xdg/xdg-ubuntu/nvim/after,~/.config/nvim/after
set scrolloff=12
set shiftwidth=4
set softtabstop=4
set noswapfile
set tabstop=4
set wildignore=**/node_modules/**
set window=53
" vim: set ft=vim :
