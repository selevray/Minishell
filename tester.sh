#!/usr/bin/env bash
# =============================================================================
#  minishell_tester — tester modulaire pour le projet minishell (42)
#  Compare la sortie et l'exit code de ton minishell avec ceux de bash.
#
#  Usage :
#    ./tester.sh                       # lance toutes les sections
#    ./tester.sh builtins redirections # lance seulement ces sections
#    ./tester.sh --list                # liste les sections disponibles
#    ./tester.sh -v quotes             # mode verbeux sur une section
#    ./tester.sh -d pipes              # affiche le diff en cas d'échec
#    ./tester.sh -s all                # stop dès le premier fail
#
#  Variables d'environnement :
#    MINISHELL=./minishell   chemin vers ton binaire (par défaut ./minishell)
#    SHELL_REF=bash          shell de référence (par défaut bash)
# =============================================================================

set -u

# ---------- Config ----------
MINISHELL="${MINISHELL:-./minishell}"
SHELL_REF="${SHELL_REF:-bash}"

# ---------- Couleurs ----------
if [[ -t 1 ]]; then
    G='\033[0;32m'; R='\033[0;31m'; Y='\033[1;33m'
    B='\033[0;34m'; C='\033[0;36m'; M='\033[0;35m'
    BOLD='\033[1m'; DIM='\033[2m'; N='\033[0m'
else
    G=''; R=''; Y=''; B=''; C=''; M=''; BOLD=''; DIM=''; N=''
fi

# ---------- État global ----------
TOTAL=0
PASSED=0
FAILED=0
FAILED_TESTS=()
CURRENT_SECTION=""

# ---------- Options ----------
VERBOSE=0
SHOW_DIFF=0
STOP_ON_FAIL=0

# ---------- Liste des sections ----------
ALL_SECTIONS=(
    builtins
    quotes
    expansion
    redirections
    heredoc
    pipes
    errors
    exit_codes
    edge
    bonus
)

# =============================================================================
#  Helpers
# =============================================================================

die() { printf "${R}✗ %s${N}\n" "$*" >&2; exit 1; }

banner() {
    printf "\n${BOLD}${B}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${N}\n"
    printf "${BOLD}${B}  %s${N}\n" "$1"
    printf "${BOLD}${B}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${N}\n"
}

section_header() {
    CURRENT_SECTION="$1"
    printf "\n${BOLD}${C}▸ Section : %s${N}\n" "$1"
    printf "${DIM}─────────────────────────────────────────${N}\n"
}

# Exécute une commande dans minishell et retourne stdout+stderr
run_minishell() {
    local cmd="$1"
    # On ajoute "exit" pour terminer proprement ; 2>&1 pour capturer stderr
    printf "%s\nexit\n" "$cmd" | "$MINISHELL" 2>&1
}

run_reference() {
    local cmd="$1"
    printf "%s\n" "$cmd" | "$SHELL_REF" 2>&1
}

# Nettoie la sortie minishell : retire les lignes qui ressemblent à un prompt.
# Personnalise ici si ton prompt est différent.
clean_output() {
    sed -E \
        -e '/^(minishell|\$|>|bash).*\$/d' \
        -e '/^minishell>/d' \
        -e '/^minishell\$>/d' \
        -e 's/^minishell[^ ]*\$ //' \
        -e 's/^> //'
}

# Cœur du tester : compare sortie et exit code
run_test() {
    local name="$1"
    local cmd="$2"
    local sort_output="${3:-0}"  # 1 pour comparer sortie triée (ex: env)

    TOTAL=$((TOTAL + 1))

    # --- minishell ---
    local ms_out ms_exit
    ms_out=$(run_minishell "$cmd")
    ms_exit=$?
    ms_out=$(printf "%s" "$ms_out" | clean_output)

    # --- référence (bash) ---
    local ref_out ref_exit
    ref_out=$(run_reference "$cmd")
    ref_exit=$?

    # Tri optionnel (utile pour env dont l'ordre peut varier)
    if [[ "$sort_output" == "1" ]]; then
        ms_out=$(printf "%s" "$ms_out"  | sort)
        ref_out=$(printf "%s" "$ref_out" | sort)
    fi

    local out_ok=0 exit_ok=0
    [[ "$ms_out"  == "$ref_out"  ]] && out_ok=1
    [[ "$ms_exit" == "$ref_exit" ]] && exit_ok=1

    if (( out_ok && exit_ok )); then
        PASSED=$((PASSED + 1))
        if (( VERBOSE )); then
            printf "  ${G}✓${N} ${DIM}[%s]${N} %s\n" "$CURRENT_SECTION" "$name"
            printf "    ${DIM}cmd: %s${N}\n" "$cmd"
        else
            printf "  ${G}✓${N} %s\n" "$name"
        fi
    else
        FAILED=$((FAILED + 1))
        FAILED_TESTS+=("[$CURRENT_SECTION] $name")
        printf "  ${R}✗${N} %s\n" "$name"
        printf "    ${DIM}cmd:${N} %s\n" "$cmd"

        if (( !out_ok )); then
            printf "    ${Y}sortie diffère${N}\n"
            if (( SHOW_DIFF )); then
                printf "    ${DIM}── attendu (bash) ──${N}\n"
                printf "%s\n" "$ref_out" | sed 's/^/      /'
                printf "    ${DIM}── obtenu (minishell) ──${N}\n"
                printf "%s\n" "$ms_out" | sed 's/^/      /'
            fi
        fi
        if (( !exit_ok )); then
            printf "    ${Y}exit code :${N} attendu=%s, obtenu=%s\n" "$ref_exit" "$ms_exit"
        fi

        if (( STOP_ON_FAIL )); then
            print_summary
            exit 1
        fi
    fi
}

# Variante qui teste uniquement l'exit code (utile quand la sortie dépend
# du système, ex : /tmp/xxx, PID, etc.)
run_test_exit_only() {
    local name="$1"
    local cmd="$2"
    local expected_exit="$3"

    TOTAL=$((TOTAL + 1))
    local ms_out ms_exit
    ms_out=$(run_minishell "$cmd")
    ms_exit=$?

    if [[ "$ms_exit" == "$expected_exit" ]]; then
        PASSED=$((PASSED + 1))
        if (( VERBOSE )); then
            printf "  ${G}✓${N} ${DIM}[%s]${N} %s ${DIM}(exit=%s)${N}\n" \
                "$CURRENT_SECTION" "$name" "$expected_exit"
        else
            printf "  ${G}✓${N} %s\n" "$name"
        fi
    else
        FAILED=$((FAILED + 1))
        FAILED_TESTS+=("[$CURRENT_SECTION] $name")
        printf "  ${R}✗${N} %s\n" "$name"
        printf "    ${DIM}cmd:${N} %s\n" "$cmd"
        printf "    ${Y}exit code :${N} attendu=%s, obtenu=%s\n" "$expected_exit" "$ms_exit"
        (( STOP_ON_FAIL )) && { print_summary; exit 1; }
    fi
}

# =============================================================================
#  Sections de tests
# =============================================================================

# ---------- 1. BUILTINS ----------
test_builtins() {
    section_header "builtins"

    # echo
    run_test "echo simple"              'echo hello'
    run_test "echo multi-args"          'echo hello world 42'
    run_test "echo -n"                  'echo -n hello'
    run_test "echo -nnnn"               'echo -nnnn hello'
    run_test "echo -n -n"               'echo -n -n hello'
    run_test "echo -nx (pas un flag)"   'echo -nx hello'
    run_test "echo sans args"           'echo'
    run_test "echo chaine vide"         'echo ""'
    run_test "echo espaces multiples"   'echo a    b    c'

    # pwd
    run_test "pwd"                      'pwd'
    run_test "pwd avec args (ignorés)"  'pwd foo bar'

    # cd
    run_test "cd /tmp"                  'cd /tmp; pwd'
    run_test "cd puis cd -"             'cd /tmp; cd /; cd -'
    run_test "cd sans argument"         'cd; pwd'
    run_test "cd ~"                     'cd ~; pwd'
    run_test "cd inexistant"            'cd /nonexistent_xyz_42 2>/dev/null; echo $?'
    run_test "cd .."                    'cd /tmp; cd ..; pwd'
    run_test "cd ."                     'cd /tmp; cd .; pwd'

    # env (sortie triée car l'ordre peut varier)
    run_test "env" 'env' 1

    # export / unset
    run_test "export + echo"            'export FOO=bar; echo $FOO'
    run_test "export sans valeur"       'export FOO; echo "[$FOO]"'
    run_test "export chaine vide"       'export FOO=""; echo "[$FOO]"'
    run_test "unset"                    'export FOO=bar; unset FOO; echo "[$FOO]"'
    run_test "unset variable absente"   'unset NEXIST_XYZ_42; echo $?'
    run_test "export réassignation"     'export FOO=1; export FOO=2; echo $FOO'
    run_test "export nom invalide"      'export 1FOO=bar 2>/dev/null; echo $?'

    # exit
    run_test_exit_only "exit 0"         'exit 0' 0
    run_test_exit_only "exit 42"        'exit 42' 42
    run_test_exit_only "exit 255"       'exit 255' 255
    run_test_exit_only "exit 256 (wrap)" 'exit 256' 0
    run_test_exit_only "exit -1 (wrap)" 'exit -1' 255
    run_test_exit_only "exit sans arg"  'exit' 0
}

# ---------- 2. QUOTES ----------
test_quotes() {
    section_header "quotes"

    run_test "simple quotes"                "echo 'hello world'"
    run_test "double quotes"                'echo "hello world"'
    run_test "simples préservent \$"        "echo '\$HOME'"
    run_test "doubles expansent \$"         'echo "$HOME"'
    run_test "quotes collées"               "echo 'a'b'c'"
    run_test "mix simple+double"            "echo 'a'\"b\"'c'"
    run_test "quote vide simple"            "echo ''"
    run_test "quote vide double"            'echo ""'
    run_test "double quote \$?"             'false; echo "$?"'
    run_test "quote dans quote (doubles)"   'echo "il a dit '"'"'salut'"'"'"'
    run_test "espaces préservés simples"    "echo '   spaces   '"
    run_test "espaces préservés doubles"    'echo "   spaces   "'
    run_test "echo quoted flag"             "echo '-n' hello"
    run_test "tab dans quote"               "echo 'a	b'"
}

# ---------- 3. EXPANSION ----------
test_expansion() {
    section_header "expansion"

    run_test "\$HOME"                    'echo $HOME'
    run_test "\$USER"                    'echo $USER'
    run_test "\$? après true"            'true; echo $?'
    run_test "\$? après false"           'false; echo $?'
    run_test "\$? après commande KO"     '/bin/false; echo $?'
    run_test "\$NEXIST vide"             'echo [$NEXIST_XYZ_42]'
    run_test "\$\$ avec texte"           'echo a$HOME' 0   # avec HOME concat
    run_test "\$VAR milieu de mot"       'export X=middle; echo start$X'\''end'\'
    run_test "\$ seul"                   'echo $'
    run_test "variable avec chiffre"     'echo $1'
    run_test "expansion dans double"     'echo "user is $USER"'
    run_test "pas d'expansion simple"    "echo '\$USER'"
    run_test "\$HOME/subdir"             'echo $HOME/subdir'
    run_test "\$? dans double quote"     'false; echo "exit=$?"'
    run_test "double \$"                 'echo $USER$USER'
    run_test "export puis expand"        'export GREET=hi; echo $GREET there'
    run_test "exit code dans cmd"        'false; echo status=$?; true; echo status=$?'
}

# ---------- 4. REDIRECTIONS ----------
test_redirections() {
    section_header "redirections"

    local tmp="/tmp/.mshtest_$$"
    # cleanup préliminaire
    rm -f "$tmp" "${tmp}2" "${tmp}3" 2>/dev/null

    run_test "> simple"                  "echo hello > $tmp; cat $tmp"
    run_test "> écrase"                  "echo first > $tmp; echo second > $tmp; cat $tmp"
    run_test ">> append"                 "echo a > $tmp; echo b >> $tmp; cat $tmp"
    run_test "< input"                   "echo contenu > $tmp; cat < $tmp"
    run_test "< fichier absent"          "cat < /no_such_file_xyz 2>/dev/null; echo \$?"
    run_test "> plusieurs"               "echo hi > ${tmp} > ${tmp}2; cat ${tmp} ${tmp}2"
    run_test "> + |"                     "echo hello > $tmp; cat $tmp | tr a-z A-Z"
    run_test "< + |"                     "echo abc > $tmp; cat < $tmp | wc -c"
    run_test ">> crée fichier"           "rm -f $tmp; echo x >> $tmp; cat $tmp"
    run_test "espaces autour de >"       "echo hi   >   $tmp; cat $tmp"
    run_test "pas d'espace autour de >"  "echo hi>$tmp; cat $tmp"
    run_test "redir sans commande"       "> $tmp; cat $tmp"
    run_test "ambiguous <>"              "cat < $tmp > ${tmp}2; cat ${tmp}2"

    rm -f "$tmp" "${tmp}2" "${tmp}3" 2>/dev/null
}

# ---------- 5. HEREDOC ----------
test_heredoc() {
    section_header "heredoc"

    run_test "heredoc simple" \
        $'cat << EOF\nhello\nEOF'
    run_test "heredoc plusieurs lignes" \
        $'cat << END\nline1\nline2\nline3\nEND'
    run_test "heredoc expansion \$USER" \
        $'cat << EOF\nuser=$USER\nEOF'
    run_test "heredoc quoted = pas d'expansion" \
        $'cat << "EOF"\nuser=$USER\nEOF'
    run_test "heredoc single-quoted" \
        $'cat << \'EOF\'\nuser=$USER\nEOF'
    run_test "heredoc + pipe" \
        $'cat << EOF | tr a-z A-Z\nhello\nEOF'
    run_test "heredoc + redirection" \
        $'cat << EOF > /tmp/.msh_hd_$$\nhello\nEOF\ncat /tmp/.msh_hd_$$\nrm /tmp/.msh_hd_$$'
    run_test "heredoc vide" \
        $'cat << EOF\nEOF'
    run_test "heredoc \$?" \
        $'false\ncat << EOF\n$?\nEOF'
}

# ---------- 6. PIPES ----------
test_pipes() {
    section_header "pipes"

    run_test "pipe simple"               'echo hello | cat'
    run_test "pipe + wc"                 'echo hello | wc -c'
    run_test "pipe tr"                   'echo hello | tr a-z A-Z'
    run_test "pipe triple"               'echo hello | cat | cat | cat'
    run_test "pipe grep"                 'echo -e "foo\nbar\nbaz" | grep ba'
    run_test "pipe builtin | extern"     'pwd | cat'
    run_test "pipe extern | builtin"     'echo $HOME | cat'
    run_test "pipe avec \$?"             'false | true; echo $?'
    run_test "pipe exit du dernier"      'true | false; echo $?'
    run_test "pipe long"                 'echo -e "a\nb\nc\nd\ne" | sort | uniq | wc -l'
    run_test "pipe + redir"              'echo hello | cat > /tmp/.mshp_$$; cat /tmp/.mshp_$$; rm /tmp/.mshp_$$'
    run_test "pipe cmd inconnue"         'nocmd_xyz_42 | echo hi 2>/dev/null'
}

# ---------- 7. ERRORS ----------
test_errors() {
    section_header "errors"

    run_test_exit_only "commande inconnue → 127"      'nocmd_xyz_42 2>/dev/null' 127
    run_test_exit_only "path inexistant → 127"        '/nowhere/nocmd 2>/dev/null' 127
    run_test_exit_only "permission denied → 126"      '/etc/hostname 2>/dev/null' 126
    run_test_exit_only "is a directory → 126"         '/tmp 2>/dev/null' 126
    run_test_exit_only "syntax error"                 'echo |' 2
    run_test_exit_only "syntax error pipe"            '| echo hi' 2
    run_test_exit_only "syntax error redir"           'echo hi >' 2
    run_test_exit_only "syntax < sans fichier"        'echo hi <' 2
    run_test_exit_only "exit avec arg non-numeric"    'exit abc 2>/dev/null' 2
    run_test "cd trop d'args"                         'cd / /tmp 2>/dev/null; echo $?'
}

# ---------- 8. EXIT CODES (cas mixtes) ----------
test_exit_codes() {
    section_header "exit_codes"

    run_test "after true"                'true; echo $?'
    run_test "after false"               'false; echo $?'
    run_test "after exit 7 sous-cmd"     '(exit 7) 2>/dev/null; echo $?'
    run_test "pipe fail"                 'false | true; echo $?'
    run_test "pipe last dicte"           'true | false; echo $?'
    run_test "cmd absente → 127"         'nocmd_xyz_42 2>/dev/null; echo $?'
    run_test "echo puis exit"            'echo a; false; echo exit=$?'
    run_test "multi chain"               'true; false; true; echo $?'
}

# ---------- 9. EDGE CASES ----------
test_edge() {
    section_header "edge"

    run_test "input vide" ""
    run_test "espaces seuls"         '     '
    run_test "tabs seuls"            $'\t\t'
    run_test "echo trop d'espaces"   '    echo     hello    world    '
    run_test "cmd suivie de ;"       'echo hi; echo ho'
    run_test "plusieurs ;"           'echo 1; echo 2; echo 3'
    run_test "echo \$\$"             'echo "split this"'   # juste un test basique
    run_test "semicolons + espaces"  'echo a ;   echo b'
    run_test "echo slash"            'echo /'
    run_test "echo star (pas glob)"  'echo "*"'
    run_test "long arg"              'echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'
    run_test "chiffres"              'echo 42 21 84'
    run_test "caractères spéciaux"   'echo "hello (world) [test] {ok}"'
}

# ---------- 10. BONUS (&&, ||, (), *) ----------
test_bonus() {
    section_header "bonus"

    run_test "&& succès"             'true && echo ok'
    run_test "&& échec"              'false && echo no'
    run_test "|| succès"             'true || echo no'
    run_test "|| échec"              'false || echo ok'
    run_test "&& et ||"              'true && echo a || echo b'
    run_test "chain mixte"           'false || true && echo yes'
    run_test "() simple"             '(echo ok)'
    run_test "() avec ;"             '(echo a; echo b)'
    run_test "() et &&"              '(true && echo ok) && echo done'
    run_test "() exit isolé"         '(exit 5); echo $?'
    run_test "wildcard *"            'cd /tmp; echo *' 1
    run_test "precedence"            'true && false || echo caught'
}

# =============================================================================
#  Runner
# =============================================================================

list_sections() {
    banner "Sections disponibles"
    for s in "${ALL_SECTIONS[@]}"; do
        printf "  ${C}•${N} %s\n" "$s"
    done
    printf "\nUtilisation : ./tester.sh %s\n" "<section...>"
}

usage() {
    cat <<'EOF'
Usage : ./tester.sh [options] [section...]

Options :
  -v, --verbose       Affiche chaque test même en succès
  -d, --show-diff     Affiche le diff sortie attendue / obtenue sur échec
  -s, --stop          S'arrête au premier échec
  -l, --list          Liste les sections disponibles
  -h, --help          Affiche cette aide

Sections : voir --list. Sans argument, toutes les sections sont lancées.

Exemples :
  ./tester.sh
  ./tester.sh builtins
  ./tester.sh -d redirections pipes
  ./tester.sh -v bonus
  MINISHELL=./my_shell ./tester.sh
EOF
}

print_summary() {
    printf "\n${BOLD}${B}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${N}\n"
    printf "${BOLD}  Résumé${N}\n"
    printf "${BOLD}${B}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${N}\n"
    printf "  Total   : %d\n" "$TOTAL"
    printf "  ${G}Passés  : %d${N}\n" "$PASSED"
    printf "  ${R}Échoués : %d${N}\n" "$FAILED"
    if (( FAILED > 0 )); then
        printf "\n${R}${BOLD}Tests échoués :${N}\n"
        for t in "${FAILED_TESTS[@]}"; do
            printf "  ${R}✗${N} %s\n" "$t"
        done
    fi
    printf "\n"
    if (( FAILED == 0 && TOTAL > 0 )); then
        printf "${G}${BOLD}🎉 Tous les tests passent !${N}\n\n"
    fi
}

run_section() {
    case "$1" in
        builtins)      test_builtins ;;
        quotes)        test_quotes ;;
        expansion)     test_expansion ;;
        redirections)  test_redirections ;;
        heredoc)       test_heredoc ;;
        pipes)         test_pipes ;;
        errors)        test_errors ;;
        exit_codes)    test_exit_codes ;;
        edge)          test_edge ;;
        bonus)         test_bonus ;;
        *) printf "${R}Section inconnue : %s${N}\n" "$1" >&2; exit 1 ;;
    esac
}

main() {
    local sections=()
    while (( $# > 0 )); do
        case "$1" in
            -v|--verbose)   VERBOSE=1 ;;
            -d|--show-diff) SHOW_DIFF=1 ;;
            -s|--stop)      STOP_ON_FAIL=1 ;;
            -l|--list)      list_sections; exit 0 ;;
            -h|--help)      usage; exit 0 ;;
            -*)             printf "Option inconnue : %s\n" "$1" >&2; usage; exit 1 ;;
            *)              sections+=("$1") ;;
        esac
        shift
    done

    [[ -x "$MINISHELL" ]] || die "Minishell introuvable ou non-exécutable : $MINISHELL
Définis la variable MINISHELL=<chemin> ou compile ton projet d'abord."

    command -v "$SHELL_REF" >/dev/null 2>&1 \
        || die "Shell de référence introuvable : $SHELL_REF"

    banner "minishell tester — cible : $MINISHELL (vs $SHELL_REF)"

    if (( ${#sections[@]} == 0 )); then
        sections=("${ALL_SECTIONS[@]}")
    fi

    for s in "${sections[@]}"; do
        run_section "$s"
    done

    print_summary
    (( FAILED == 0 )) && exit 0 || exit 1
}

main "$@"
