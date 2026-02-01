;;; arl-mode.el --- ARL mode for Emacs               -*- lexical-binding: t; -*-

;; Copyright (C) 2026  Aryadev Chavali

;; Author: Aryadev Chavali <aryadev@aryadevchavali.com>
;; Keywords:

;; Copyright (C) 2026 Aryadev Chavali

;; This program is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
;; FOR A PARTICULAR PURPOSE.  See the MIT License for details.

;; You may distribute and modify this code under the terms of the MIT License,
;; which you should have received a copy of along with this program.  If not,
;; please go to <https://opensource.org/license/MIT>.

;;; Commentary:

;;

;;; Code:

(defvar arl-mode-comments '(?\; ";;" ("#|" . "|#")))
(defvar arl-mode-keywords '("if" "then" "else"))
(defvar arl-mode-expressions '(("\".*\"" . font-lock-string-face)))
(defvar arl-mode-automode-list '("\\.arl"))

(define-derived-mode arl-mode
  arl-mode-comments
  arl-mode-keywords
  arl-mode-expressions
  arl-mode-automode-list
  nil)

(provide 'arl-mode)
;;; arl-mode.el ends here
